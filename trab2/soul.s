# Tratador de interrupções
int_handler:
    # Salva contexto
    csrrw s6, mscratch, s6 # troca valor de s6 com mscratch
    sw a0, 0(s6)
    sw a1, 4(s6)
    sw a2, 8(s6)
    sw a3, 12(s6)
    sw a4, 16(s6)
    sw a5, 20(s6)
    sw a6, 24(s6)
    sw a7, 28(s6)
    sw s1, 32(s6)
    sw s2, 36(s6)
    sw s3, 40(s6)
    sw s5, 44(s6)



    # Identificando a causa
    csrr a1, mcause # lê a causa da exceção                
    bgez a1, exception # desvia se não for uma interrupção 
    
    # Trata interrupção
    interruption:
        li a7, 0xFFFF0104
        lb a0, 0(a7)
        beq zero, a0, int_end2
        li a1, 100
        la a2, timer
        lw a3, 0(a2)
        add a3, a3, a1
        sw a3, 0(a2)
        li a7, 0xFFFF0104
        sb zero, 0(a7)
        li a7, 0xFFFF0100
        sw a1, 0(a7)
        j int_end2
        

    # Trata exceção
    exception:
        lw s2, 28(s6)
        li s3, 16
        beq s2, s3, r_ultrasonic_sensor
        li s3, 17
        beq s2, s3, s_servo_angles
        li s3, 18
        beq s2, s3, s_engine_torque
        li s3, 19
        beq s2, s3, r_gps
        li s3, 20
        beq s2, s3, r_gyroscope
        li s3, 21
        beq s2, s3, g_time
        li s3, 22
        beq s2, s3, s_time
        li s3, 64
        beq s2, s3, _write
        j int_end
        _write:
            lb a0, 0(s6)
            lw a1, 4(s6)
            lb a2, 8(s6)
            li a3, 0xFFFF0109
            addi a2, a2, -1
            loop5:
                lb a4, 0(a1)
                sb a4, 0(a3)
                sb a0, -1(a3)
                loop4:
                    li a5, 0xFFFF0108
                    lb a6, 0(a5)
                    bne a6, zero, loop4
                beq a2, zero, int_end
                addi a2, a2, -1
                addi a1, a1, 1
                j loop5

        r_ultrasonic_sensor:
            li s5, 0xFFFF0020
            sw zero, 0(s5)
            loop:
                li s5, 0xFFFF0020
                lw s2, 0(s5)
                beq s2, zero, loop
            li s5, 0xFFFF0024
            lw a0, 0(s5)
            li a1, 600
            bge a0, a1, negative
            sw a0, 0(s6)
            j int_end
            negative:
                li a0, 0xFFFF
                sw a0, 0(s6)
                j int_end

        s_servo_angles:
            lb a1, 0(s6)
            lb a2, 4(s6)
            li s3, 0
            beq a1, s3, base
            addi s3, s3, 1
            beq a1, s3, mid
            addi s3, s3, 1
            beq a1, s3, top
            j int_end
            base:
                li s5, 0xFFFF001E
                sb a2, 0(s5)
                j int_end
            mid:
                li s5, 0xFFFF001D
                sb a2, 0(s5)
                j int_end
            top:
                li s5, 0xFFFF001C
                sb a2, 0(s5)
                j int_end


        s_engine_torque:
            lh a1, 0(s6)
            lh a2, 4(s6)
            bne a1, zero, engineR
            engineL:
            li a7, 0xFFFF001A
            sh a2, 0(a7)
            j int_end
            engineR:
                li a7, 0xFFFF0018
                sh a2, 0(a7)
                j int_end

        r_gps:
            lw a1, 0(s6)
            li s5, 0xFFFF0004
            sw zero, 0(s5)
            loop2:
                li s5, 0xFFFF0004
                lw s3, 0(s5)
                beq s3, zero, loop2
                li s5, 0xFFFF0008
                lw a5, 0(s5)
                sw a5, 0(a1)
                li s5, 0xFFFF000C
                lw a5, 0(s5)
                sw a5, 4(a1)
                li s5, 0xFFFF0010
                lw a5, 0(s5)
                sw a5, 8(a1)
            j int_end

        r_gyroscope:
            lw a1, 0(s6)
            li s5, 0xFFFF0004
            sw zero, 0(s5)
            loop3:
                li s5, 0xFFFF0004
                lw s3, 0(s5)
                beq s3, zero, loop3
                li s5, 0xFFFF0014
                lw a5, 0(s5)
                mv s3, a5
                li s2, 10
                srli s3, s3, 10
                slli s3, s3, 10
                sub a6, a5, s3
                sw a6, 8(a1)
                srli a5, a5, 10
                mv s3, a5
                srli s3, s3, 10
                slli s3, s3, 10
                sub a6, a5, s3
                sw a6, 4(a1)
                srli a5, a5, 10
                mv s3, a5
                srli s3, s3, 10
                slli s3, s3, 10
                sub a6, a5, s3
                mv a6, a5
                sw a6, 0(a1)
            j int_end
            
        g_time:
            la a1, timer
            lw a0, 0(a1)
            sw a0, 0(s6)
            j int_end

        s_time:
            lw a1, 0(s6)
            sw a1, timer, a7
            j int_end

    int_end:
        # Ajustando MEPC para retornar de uma chamada de sistema
        csrr s4, mepc # carrega endereço de retorno
        # (endereço da instrução que invocou a syscall)
        addi s4, s4, 4 # soma 4 no endereço de retorno
        # (para retornar após a ecall)
        csrw mepc, s4 # armazena endereço de retorno de volta no mepc
    int_end2:
        # Recupera contexto
        lw s5, 44(s6)
        lw s3, 40(s6)
        lw s2, 36(s6)
        lw s1, 32(s6)
        lw a7, 28(s6)
        lw a6, 24(s6)
        lw a5, 20(s6)
        lw a4, 16(s6)
        lw a3, 12(s6)
        lw a2, 8(s6)
        lw a1, 4(s6)
        lw a0, 0(s6)
        csrrw s6, mscratch, s6 # troca valor de s6 com mscratch
        mret # retorna do tratador



# Configuração do sistema (feita durante a operação de reset)
.globl _start
_start:
    li s5, 0xFFFF001A
    sw zero, 0(s5)
    li s5, 0xFFFF0018
    sw zero, 0(s5)
    li s2, 31
    li s5, 0xFFFF001E
    sb s2, 0(s5)
    addi s2, s2, 47
    li s5, 0xFFFF001C
    sb s2, 0(s5)
    addi s2, s2, 2
    li s5, 0xFFFF001D
    sb s2, 0(s5)
    li s2, 100
    li s5, 0xFFFF0100
    sw s2, 0(s5)
    

    # Configura o tratador de interrupções
    la s1, int_handler # Grava o endereço do rótulo int_handler
    csrw mtvec, s1 # no registrador mtvec

    # Habilita Interrupções Global
    csrr s2, mstatus # Seta o bit 7 (MPIE)
    ori s2, s2, 0x80 # do registrador mstatus
    csrw mstatus, s2

    # Habilita Interrupções Externas
    csrr s2, mie # Seta o bit 11 (MEIE)
    li s3, 0x800 # do registrador mie
    or s2, s2, s3
    csrw mie, s2

    # Ajusta o mscratch
    la s2, reg_buffer # Coloca o endereço do buffer para salvar
    csrw mscratch, s2 # registradores em mscratch

    la sp, pilha_usuario
    li s1, 48000
    add sp, sp, s1

    # Muda para o Modo de usuário
    csrr s2, mstatus # Seta os bits 11 e 12 (MPP)
    li s3, ~0x1800 # do registrador mstatus
    and s2, s2, s3 # com o valor 00
    csrw mstatus, s2
    la s1, main # Grava o endereço do rótulo user
    csrw mepc, s1 # no registrador mepc
    mret # PC <= MEPC; MIE <= MPIE; Muda modo para MPP

# Trecho de código do usuário
.align 4

timer: .skip 160

reg_buffer: .skip 48000

pilha_usuario: .skip 48000