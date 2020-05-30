.globl _start
# Função para obtenção do tempo atual. ao <= tempo atual
time_now:
  la a0, buffer_timeval
  la a1, buffer_timerzone
  li a7, 169 # chamada de sistema gettimeofday
  ecall
  la a0, buffer_timeval
  lw t1, 0(a0) # tempo em segundos
  lw t2, 8(a0) # fração do tempo em microssegundos
  li t3, 1000
  mul t1, t1, t3
  div t2, t2, t3
  add a0, t2, t1
  ret

# Função para tocar o canal a0 canal do sintetizador
play_channel:
  slli a1, a0, 2     # canal = canal*4 // calcula posicao do canal no vetor C
  add a7, s8, a1     # soma deslocamento na matriz M 
  add a4, s4, a1     # soma deslocamento no vetor P
  add a5, s5, a1     # soma deslocamento no vetor P2
  add a3, s3, a1     # soma deslocamento no vetor C
  lw t1, 0(a7)       # lê matriz
  srli t2, t1, 16    # t2 = frequencia||velocidade
  beq t2, zero, end  # se zero, não tocar nada
  lw t3, 0(a4)       # t3 = (frequencia||velocidade) tocadas por último
  bne t2, t3, tocar; # se diferente, trocou a nota, então deve tocar
  lw t3, 0(a5)       # Tempo de termino do que está tocando
  ble s7, t3, end;   # Se ainda não parou de tocar anterior, não deve tocar novamente
  tocar:
    sw t2, 0(a4)     # Atualiza (frequencia||velocidade) em P
    and t2, t1, s6   # t2 = delay
    add t2, t2, s7   # Calcula até quando ir tocar
    sw t2, 0(a5)     # Armazena no vetor P2
    mv a2, a0        # Define o Canal
    lw a0, 0(a3)     # Carrega instrumento do canal
    mv a1, t1        # move (frequencia||velocidade||delay) para a1
    li a7, 2048      # chamada de sistema
    ecall
    end:
    ret

# Função para tocar todos os canais do sintetizador
play_all_channels:
  li a0, 15
  mv s10, ra
  while_channels: 
    mv s9, a0
    jal play_channel
    add a0, s9, -1
    bge a0, zero, while_channels
  mv ra, s10
  ret

.text
_start:
############  Implemente o Parser aqui  #############
  la s1, line
  bpmticks:
  li a0, 0 # file descriptor = 0 (stdin)
  la a1, in #  buffer
  li a2, 1 # size 
  li a7, 63 # syscall read (63)
  ecall

  lb a1, in
  sb a1, 0(s1)
  addi s1, s1, 1
  li t1, 10
  bne a1, t1, bpmticks

  la s1, line
  jal decifra
  sw s2, bpm, t1
  sub s2, s2, s2
  addi s1, s1, 1
  jal decifra
  sw s2, ticks, t1
  sub s2, s2, s2


  midi:
  la s1, line
  input:
    li a0, 0 # file descriptor = 0 (stdin)
    la a1, in #  buffer
    li a2, 1 # size 
    li a7, 63 # syscall read (63)
    ecall


  lb a1, in
  sb a1, 0(s1)
  addi s1, s1, 1
  li t1, 10
  bne a1, t1, input



  lb a1, line
  li t1, 72
  beq a1, t1, midi
  li t1, 80
  beq a1, t1, program
  jal nota




  program:
    la s1, line + 10
    jal decifra
    sw s2, canal, t1
    sub s2, s2, s2
    addi s1, s1, 1
    jal decifra
    sw s2, instrumento, t1
    sub s2, s2, s2


    la s5, C
    lw t1, canal
    addi t1, t1, -1
    slli t1, t1, 2
    add s5, s5, t1
    lw t1, instrumento
    sw t1, 0(s5)
    j midi
  
  nota:
    la s1, line
    jal decifra
    sw s2, tinicio, t1
    sub s2, s2, s2
    lb a5, 0(s1)
    li t1, 10
    beq a5, t1, end2
    addi s1, s1, 1
    jal decifra
    sw s2, tfim, t1
    sub s2, s2, s2
    addi s1, s1, 1
    jal decifra
    sw s2, track, t1
    sub s2, s2, s2
    addi s1, s1, 1
    jal decifra
    sw s2, canal2, t1
    sub s2, s2, s2
    addi s1, s1, 1
    jal decifra
    sw s2, freq, t1
    sub s2, s2, s2
    addi s1, s1, 1
    jal decifra
    sw s2, vel, t1
    sub s2, s2, s2





    la s5, M
    lw s4, tfim
    lw a2, bpm
    lw a3, ticks
    li t1, 10
    li t2, 6000
    lw s3, tinicio
    div a3, a3, t1
    mul s3, s3, t2
    div s3, s3, a2
    div s3, s3, a3
    slli s6, s3, 6
    add s5, s5, s6
    lw a1, canal2
    addi a1, a1, -1
    slli a1, a1, 2
    add s5, s5, a1
    mul s4, s4, t2
    div s4, s4, a2
    div s4, s4, a3

    while:
        lb a1, freq
        sb a1, 3(s5)
        lb a2, vel
        sb a2, 2(s5)
        sub a3, s4, s3
        sh a3, 0(s5)
        addi s3, s3, 1
        addi s5, s5, 64
        bne s3, s4, while

    j midi

  

decifra: # endereco em s1
  li t2, 33
  li t1, 10
  lp:
      lb a1, 0(s1)
      blt a1, t2, retorna
      addi a1, a1, -48
      mul s2, s2, t1
      addi s1, s1, 1
      add s2, s2, a1
      j lp

  retorna:
      ret

end2:
  j play



in: .skip 32

line: .skip 32

canal: .skip 4

instrumento: .skip 4

tinicio: .skip 20

tfim: .skip 20

track: .skip 20

canal2: .skip 4

freq: .skip 4

vel: .skip 4

bpm: .skip 4

ticks: .skip 32




############        Fim do Parser       #############

play:
  jal time_now 
  mv s1, a0 # s1 recebe o tempo atual em ms
  la s2, M
  la s3, C
  la s4, P
  la s5, P2
  li s6, 0xffff
while_1:
  jal time_now        # a0 recebe o tempo atual em ms
  sub s7, a0, s1      # s7 recebe o tempo desde o início da função
  slli a1, s7, 6      # a1 = a1*64 // calculo do deslocamento para linha
  add s8, s2, a1      #s8 = &M + a1 // endereço da linha a ser processada
  jal play_all_channels
j while_1

.data 
.align 4
buffer_timeval: .skip 12
buffer_timerzone: .skip 12
P:  .skip 64
P2: .skip 64
C:  .skip 64
.comm	M,19200000,4 # Reserva um espaço de memória com 19.2MB iniciado no endereço marcalo pelo rótulo M

