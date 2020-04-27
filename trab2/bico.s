.globl set_torque
.globl set_engine_torque
.globl set_head_servo
.globl get_us_distance
.globl get_current_GPS_position
.globl get_gyro_angles
.globl get_time
.globl set_time
.globl puts


######### ENGINES ##########

#int engine_1 salvo em a0
#int engine_2 salvo em a1
#retorna int em a5
set_torque:
    li s1, 101
    li s2, -100
    bge a0, s1, else1 # if t0 >= t1 then target
    blt a0, s2, else1 # if t0 < t1 then target
    bge a1, s1, else1 # if t0 >= t1 then target
    blt a1, s2, else1 # if t0 < t1 then target
    mv s1, a0
    mv s2, a1
    mv a1, s1
    li a0, 0
    li a7, 18
    ecall
    li a0, 1
    mv a1, s2
    li a7, 18
    ecall
    li a5, 0
    ret
    else1:
        li a5, -1
        ret


#int engine_id salvo em a0
#int torque salvo em a1
#retorna int em a5
set_engine_torque:
    li s1, 101
    li s2, -100
    li s3, 2
    bge a0, s3, else21 # if a0 >= 2 then else21
    blt a0, zero, else21
    bge a1, s1, else22 # if t0 >= t1 then target
    blt a1, s2, else22 # if t0 < t1 then target
    # chama a syscall 18 ########################
    li a7, 18
    ecall
    li a5, 0
    ret
    else22:
        li a5, -2
        ret
    else21:
        li a5, -1
        ret


#int servo_id salvo em a0
#int angle salvo em a1
#retorna int em a5
set_head_servo:
    li s1, 1
    li s2, 2
    beq a0, zero, _base # if a0 == t1 then target
    beq a0, s1, _mid
    beq a0, s2, _top
    j else32
    _base:
    li s1, 117
    li s2, 16
    bge a1, s1, else31 
    blt a1, s2, else31
    j sysc
    _mid:
    li s1, 91
    li s2, 52
    bge a1, s1, else31 
    blt a1, s2, else31
    j sysc    
    _top:
    li s1, 157
    bge a1, s1, else31 
    blt a1, zero, else31
    # chama a syscall 17 ########################
    sysc:
    li a7, 17
    ecall
    li a5, 0
    ret
    else32:
        li a5, -1
        ret
    else31:
        li a5, -2
        ret

############################


######### SENSORS ##########

#retorna unsigned short em a0
get_us_distance:
    # chama a syscall 16 ########################
    li a7, 16
    ecall
    ret

#Vector3 pos salvo em a0
get_current_GPS_position:
    # chama a syscall 19 ########################
    li a7, 19
    ecall
    ret

#Vector3 angles salvo em a0
get_gyro_angles:
    # chama a syscall 20 ########################
    li a7, 20
    ecall
    ret

########## TIMER ###########

#retorna unsigned int em a5
get_time:
    # chama a syscall 21 ########################
    li a7, 21
    ecall
    mv a5, a0
    ret

#unsigned int t salvo em a0
set_time:
    # chama a syscall 22 ########################
    li a7, 22
    ecall
    ret

############################





########### UART ###########
#descritor do arquivo em a0
#const char* salvo em a1
#numero de bytes a serem escritos em a2
puts:
    #chama a syscall 64 #########################
    li a3, 0
    loop6:
        lb a4, 0(a0)
        addi a3, a3, 1
        addi a0, a0, 1
        bne a4, zero, loop6
    li a5, 10
    sb a5, 0(a0)
    sub a0, a0, a3
    addi a3, a3, 1
    mv a1, a0
    li a0, 1
    mv a2, a3
    li a7, 64
    ecall
    ret
############################