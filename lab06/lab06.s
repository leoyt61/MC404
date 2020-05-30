.globl _start

_start:
    li a0, 0 # file descriptor = 0 (stdin)
    la a1, in #  buffer
    li a2, 12 # size (lendo apenas 1 byte, mas tamanho é variável)
    li a7, 63 # syscall read (63)
    ecall

    li a0, 0 # file descriptor = 0 (stdin)
    la a1, in2 #  buffer
    li a2, 20 # size (lendo apenas 1 byte, mas tamanho é variável)
    li a7, 63 # syscall read (63)
    ecall
    ####################################################################

    lb a1, in
    sw a1, sign_yb, t1
    lb a1, in+6
    sw a1, sign_xc, t1
    

    #Bloco para colocar os valores decimais nos respectivos rotulos
    lb s1, in+1
    lb s2, in+2
    lb s3, in+3
    lb s4, in+4
    jal decifrar_num
    sw s7, yb, t1

    lb s1, in+7
    lb s2, in+8
    lb s3, in+9
    lb s4, in+10
    jal decifrar_num
    sw s7, xc, t1

    lb s1, in2
    lb s2, in2+1
    lb s3, in2+2
    lb s4, in2+3
    jal decifrar_num
    sw s7, ta, t1
    
    lb s1, in2+5
    lb s2, in2+6
    lb s3, in2+7
    lb s4, in2+8
    jal decifrar_num
    sw s7, tb, t1

    lb s1, in2+10
    lb s2, in2+11
    lb s3, in2+12
    lb s4, in2+13
    jal decifrar_num
    sw s7, tc, t1

    lb s1, in2+15
    lb s2, in2+16
    lb s3, in2+17
    lb s4, in2+18
    jal decifrar_num
    sw s7, tr, t1
    

    #bloco para calcular da, db e dc
    li a1, 3
    li a2, 10
    lh s7, tr
    lh s1, ta
    sub s1, s7, s1
    mul s1, s1, a1
    div s1, s1, a2
    sw s1, da, t1
    lh s1, tb
    sub s1, s7, s1
    mul s1, s1, a1
    div s1, s1, a2
    sw s1, db, t1
    lh s1, tc
    sub s1, s7, s1
    mul s1, s1, a1
    div s1, s1, a2
    sw s1, dc, t1


    #bloco para calcular y
    lh s1, da
    mul s1, s1, s1
    lh s2, yb
    mul s2, s2, s2
    add s1, s1, s2
    lh s2, db
    mul s2, s2, s2
    jal sign_compareY
    jal subtrair
    li a1, 2
    lh s2, yb
    mul s2, s2, a1
    div s7, s7, s2
    sw s7, res_y, t1

    #bloco para calcular x
    lh s1, da
    mul s1, s1, s1
    lh s2, res_y
    mul s2, s2, s2
    jal subtrair
    mv s1, s7
    jal sqrt
    sw s2, res_x, t1

    #bloco para descobrir o sinal de x
    lh s1, res_x
    lh s2, xc
    lh s3, res_y
    mul s3, s3, s3
    lh s4, dc
    mul s4, s4, s4
    jal subtrair
    jal sign_compareX

    #bloco para juntar x e y com sinais
    lh a1, sign_x
    sb a1, 0(s5)
    lh a1, res_x
    jal transformar_char
    sb s1, 1(s5)
    sb s2, 2(s5)
    sb s3, 3(s5)
    sb s4, 4(s5)
    li a1, 32 #espaco
    sb a1, 5(s5)
    lh a1, sign_y
    sb a1, 6(s5)
    lh a1, res_y
    jal transformar_char
    sb s1, 7(s5)
    sb s2, 8(s5)
    sb s3, 9(s5)
    sb s4, 10(s5)
    li a1, 10
    sb a1, 11(s5)
    sb s5, out, t1


    ####################################################################

    li a0, 1 # file descriptor = 1 (stdout)
    lh a1, out  #buffer
    li a2, 12 # size (lendo apenas 1 byte, mas tamanho é variável)
    li a7, 64 # syscall write (64)
    ecall



end:
    li a0, 0 # exit code
    li a7, 93 # syscall exit
    ecall







decifrar_num: #caracteres salvos em s1, s2, s3, s4
    li a1, 48 #48 salvo em a1
    sub s1, s1, a1 # s1 = valor decimal
    li a2, 1000 # 1000 fica salvo em a2
    mul s7, s1, a2 
    sub s2, s2, a1
    li a2, 100
    mul t1, s2, a2
    add s7, s7, t1
    sub s3, s3, a1
    li a2, 10
    mul t1, s3, a2
    add s7, s7, t1
    sub s4, s4, a1
    add s7, s7, s4
    ret             # resultado fica salvo em s7


transformar_char: # decimal salvo em a1
    li a2, 1000
    div s1, a1, a2
    mul a2, s1, a2
    sub a1, a1, a2
    li a2, 100
    div s2, a1, a2
    mul a2, s2, a2
    sub a1, a1, a2
    li a2, 10
    div s3, a1, a2
    mul a2, s3, a2
    sub a1, a1, a2
    mv s4, a1
    addi s1, s1, 48
    addi s2, s2, 48
    addi s3, s3, 48
    addi s4, s4, 48
    ret              # resultados salvos em s1, s2, s3 e s4


sqrt:   #decimal salvo em s1 - y = s1, k = s2
    li a1, 2
    div s2, s1, a1
    li a2, 10 # contador
    li a3, 1 # constante
    loop:
        sub a2, a2, a3
        div s3, s1, s2
        add s3, s3, s2
        div s2, s3, a1
        blt zero, a2, loop
    ret


sign_compareY: # valores salvos em s1, s2 e salva no rotulo sign_y (s1 = (da2 + Yb2), s2 = db2)
    li a1, 43
    blt s2, s1, positive # if t0 < t1 then target
    lh s3, sign_yb
    beq s3, a1, negative
    sw a1, sign_y, t1
    ret
    negative:
        li a2, 45
        sw a2, sign_y, t1
        ret
    positive:
        lh a1, sign_yb
        sw a1, sign_y, t1
        ret 

sign_compareX:  # valores salvos em s1, s2, s3, s4 (s1 = x, s2= Xc, s3 = y2, s4 = dc2
    mul s7, s7, s7
    add s7, s7, s3
    sub s7, s7, s4 # somatorio 1 salvo em s7 para posterior comparacao
    add s1, s1, s2
    mul s1, s1, s1
    add s1, s1, s3
    sub s1, s1, s4 #somatorio 2 salvo em s1 para comparacao
    bge s1, s7, maior
    li s7, 43
    sw s7, sign_x, t1
    ret
    maior:
        li s1, 45
        sw s1, sign_x, t1
        ret
        

subtrair:    # decimais salvos em s1 e s2
    blt s1, s2, inverte
    sub s7, s1, s2
    ret
    inverte:
        sub s7, s2, s1
        ret #valor da subtracao fica salvo em s7



in:
.skip 12
in2:
.skip 20

res_y:
.skip 4
res_x:
.skip 4

yb: 
.skip 4
xc:
.skip 4
ta:
.skip 4
tb:
.skip 4
tc:
.skip 4
tr:
.skip 4
sign_xc:
.skip 4
sign_yb:
.skip 4

sign_y:
.skip 4
sign_x:
.skip 4

da:
.skip 4
db:
.skip 4
dc:
.skip 4

out:

