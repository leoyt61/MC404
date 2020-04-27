.globl ajudaORobinson

ajudaORobinson:
    jal inicializaVisitados

    lb a1, xRob # 3
    lb a2, yRob # 1
    la s5, Mx
    la s1, Mx
    la s2, My
    sb a1, 0(s1)
    sb a2, 0(s2)

    
    li t1, 1
    li t2, 0
    la s3, N
    sb t1, 0(s3)
    sb t2, 1(s3)
    addi s3, s3, 2
    li t1, 1
    li t2, 1
    sb t1, 0(s3)
    sb t2, 1(s3)
    addi s3, s3, 2
    li t1, 0
    li t2, 1
    sb t1, 0(s3)
    sb t2, 1(s3)
    addi s3, s3, 2
    li t1, -1
    li t2, 1
    sb t1, 0(s3)
    sb t2, 1(s3)
    addi s3, s3, 2
    li t1, -1
    li t2, 0
    sb t1, 0(s3)
    sb t2, 1(s3)
    addi s3, s3, 2
    li t1, -1
    li t2, -1
    sb t1, 0(s3)
    sb t2, 1(s3)
    addi s3, s3, 2
    li t1, 0
    li t2, -1
    sb t1, 0(s3)
    sb t2, 1(s3)
    addi s3, s3, 2
    li t1, 1
    li t2, -1
    sb t1, 0(s3)
    sb t2, 1(s3)
    jal recursao



print:
    la s1, Mx
    la s2, My
    la s3, out
    li a5, 0
    addi s6, s6, 48
    addi s7, s7, 48
    lp:
        addi a5, a5, 4
        lb a3, 0(s1)
        lb a4, 0(s2)
        addi a3, a3, 48
        addi a4, a4, 48
        addi s1, s1, 1
        addi s2, s2, 1
        bne a3, s6, lp
        bne a4, s7, lp
    la s1, Mx
    la s2, My 
    la s3, out
    add s3, s3, a5
    lp2:
        lb a3, 0(s1)
        lb a4, 0(s2)
        addi a3, a3, 48
        addi a4, a4, 48
        sb a3, 0(s3)
        li t1, 32
        sb t1, 1(s3)
        sb a4, 2(s3)
        li t1, 10
        sb t1, 3(s3)
        addi s3, s3, -4
        addi s1, s1, 1
        addi s2, s2, 1
        bne a3, s6, lp2
        bne a4, s7, lp2


    li a0, 1 # file descriptor = 1 (stdout)
    la a1, out  #buffer
    mv a2, a5# size (lendo apenas 1 byte, mas tamanho é variável)
    li a7, 64 # syscall write (64)
    ecall
    li a0, 0 # exit code
    li a7, 93 # syscall exit
    ecall





recursao:
    addi sp, sp, -4
    sw ra, 0(sp)
    lb a0, 0(s1)
    lb a1, 0(s2)
    lb s6, xLoc
    lb s7, yLoc
    li a5, 0
    jal visitaCelula
    beq a0, s6, segue
    j else
    segue:
    beq a1, s7, print
    else:
        la s4, N
        li a6, 8
        foreachNofC:
        lb a0, 0(s1)
        lb a1, 0(s2)
        addi a6, a6, -1
        lb t1, 0(s4)
        lb t2, 1(s4)
        add a0, t1, a0
        add a1, t2, a1
        addi s4, s4, 2
        li a5, 0
        jal daParaPassar
        lb a0, 0(s1)
        lb a1, 0(s2)
        add a0, t1, a0
        add a1, t2, a1
        beq a5, zero, volta
        li a5, 0
        jal foiVisitado
        lb a0, 0(s1)
        lb a1, 0(s2)
        add a0, t1, a0
        add a1, t2, a1
        bne a5, zero, volta
        addi s1, s1, 1
        addi s2, s2, 1
        sb a0, 0(s1)
        sb a1, 0(s2)
        jal recursao
        volta:
        bne a6, zero, foreachNofC
        addi s1, s1, -1
        addi s2, s2, -1
        lw ra, 0(sp)
        addi sp, sp, 4
        ret
        
        


Mx: .skip 64
My: .skip 64
N: .skip 16
out: