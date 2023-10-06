    .data

maxIter:    .word 255

zx:         .word 0

zy:         .word 0

tmp:        .word 0

i:          .word 0
x:          .word 0
y:          .word 0

color:      .short 0

fif_hun:    .word 1500
ten_hun:    .word 1000
four_mil:   .word 4000000


    .text
    .global juliaset

juliaset:
    stmfd sp!,{fp,lr}
    ldr r4, =x
    mov r5, #0
    str r5, [r4]
    ldr r4, =y
    mov r5, #0
    str r5, [r4]

loop1:
    ldr r4, =x
    ldr r4, [r4]
    cmp r4, r2
    bge loop1_end @ if(>=)
    blt loop2

loop2:
    ldr r5, =y
    ldr r5, [r5]
    cmp r5, r3
    bge loop2_end @ if(>=)

    @ ==========================================
    @ zx = 1500 * (x-(width>>1))/(width>>1)

    mov r7, r2        @ r7 = width
    mov r7, r7, asr #1  @ r7 = width >> 1

    ldr r4, =x
    ldr r4, [r4]
    sub r6, r4, r7      @ r6 = x - ( width >> 1 )
    ldr r8, =fif_hun    @ r8 = 1500
    ldr r8, [r8]
    mul r6, r8, r6      @ r6 = r8 * r6
    mov r4, r0
    mov r5, r1
    mov r0, r6
    mov r1, r7
    mov r8, r2
    mov r9, r3
    bl __aeabi_idiv
    ldr r6, =zx
    str r0, [r6]
    mov r0, r4
    mov r1, r5
    mov r2, r8
    mov r3, r9

    @================================================
    @ zy = 1000 * (y-(height>>1))/(height>>1)

    mov r7, r3        @ r7 = height
    mov r7, r7, asr #1  @ r7 = height >> 1

    ldr r5, =y
    ldr r5, [r5]
    sub r6, r5, r7      @ r6 = y - ( height >> 1 )
    ldr r8, =ten_hun    @ r8 = 1000
    ldr r8, [r8]
    mul r6, r8, r6      @ r6 = r8 * r6
    mov r4, r0
    mov r5, r1
    mov r8, r2
    mov r9, r3
    mov r0, r6
    mov r1, r7
    bl __aeabi_idiv
    ldr r6, =zy
    str r0, [r6]
    mov r0, r4
    mov r1, r5
    mov r2, r8
    mov r3, r9

    @===============================================

    ldr r7, =maxIter
    ldr r7, [r7]
    ldr r6, =i
    str r7, [r6]

    @===============================================

loop3:
    ldr r6, =zx         @ r6 = zx
    ldr r6, [r6]
    mul r8, r6, r6      @ r8 = zx * zx
    ldr r7, =zy         @ r7 = zy
    ldr r7, [r7]
    mul r9, r7, r7      @ r9 = zy * zy
    add r4, r8, r9      @ r4 = zx*zx + zy*zy
    ldr r5, =four_mil
    ldr r5, [r5]
    cmp r4, r5          @ r4 < 4000000
    bge loop3_end

    ldr r6, =i
    ldr r6, [r6]
    cmp r6, #0          @ i > 0
    ble loop3_end

    @ ===========================================
    @ tmp = ( zx*zx - zy*zy ) / 1000 + cX

    sub r6, r8, r9      @ r6 = zx * zx - zy * zy
    ldr r4, =ten_hun    @ r4 = 1000
    ldr r4, [r4]
    mov r5, r0
    mov r7, r1
    mov r8, r2
    mov r9, r3
    mov r0, r6
    mov r1, r4
    bl __aeabi_idiv    @ tmp = tmp / 1000
    ldr r6, =tmp
    str r0, [r6]
    mov r0, r5
    mov r1, r7
    mov r2, r8
    mov r3, r9
    ldr r6, =tmp
    ldr r6, [r6]
    add r7, r6, r0      @ tmp = tmp + cX
    ldr r6, =tmp
    str r7, [r6]
    @ ===========================================
    @ zy = ( 2 * zx * zy ) / 1000 + cy

    ldr r7, =zy         @ r7 = zy
    ldr r7, [r7]
    ldr r6, =zx         @ r6 = zx
    ldr r6, [r6]
    mul r7, r7, r6      @ zy = zy * zx
    mov r7, r7, lsl #1   @ zy = zy * 2

    mov r4, r0
    mov r5, r1
    mov r8, r2
    mov r9, r3
    mov r0, r7
    ldr r1, =ten_hun
    ldr r1, [r1]
    bl __aeabi_idiv     @ zy = zy / 1000
    ldr r7, =zy
    str r0, [r7]
    mov r0, r4
    mov r1, r5
    mov r2, r8
    mov r3, r9
    ldr r7, =zy
    ldr r7, [r7]
    add r8, r7, r1, lsl #0      @ zy = zy + cY
    ldr r7, =zy
    str r8, [r7]

    ldr r8, =tmp
    ldr r8, [r8]
    ldr r6, =zx
    str r8, [r6]    @ zx = tmp
    ldr r4, =i
    ldr r4, [r4]
    sub r4, r4, #1      @ i--
    ldr r5, = i
    str r4, [r5]
    b loop3


loop3_end:

    @===============================================
    @ color = ~(((i&0xff)<<8) | (i&oxff)) & 0xffff
    ldr r4, =i
    ldr r4, [r4] @ r5 = i&0xff
    mov r4, r4, lsl #8  @ r6 = i&0xff << 8
    uxth r6, r4

    ldr r4, =i
    ldr r4, [r4]
    uxth r4, r4
    orr r5, r6, r4      @ r7 = (r6|r5)
    ldr r4, =color
    strh r5, [r4]
    ldr r5, =color
    ldrh r5, [r5]
    mvn r5, r5         @ r4 = !r4
    ldr r6, =color
    strh r5, [r6]

    @===============================================
    @ frame[y][x] = color
    ldr r7, =y
    ldr r7, [r7]
    ldr r5, =x
    ldr r5, [r5]
    mul r6, r7, r2
    add r6, r6, r5              @ r6 = y*640 + x
    mov r6, r6, lsl #1
    add r6, r6, #8
    ldr r4, =color
    ldrh r4, [r4]
    strh r4, [sp, r6]    @ frame[y][x]=color

    @===============================================

    ldr r4, =y
    ldr r5, [r4]
    add r5, r5, #1
    str r5, [r4]
    b loop2

loop2_end:
    ldr r4, =x
    ldr r5, [r4]
    add r5, r5, #1
    str r5, [r4]

    mov r5, #0
    ldr r4, =y
    str r5, [r4]
    b loop1

loop1_end:
    mov r4, r14
    adds r14, r0, r15
    mov r14, r4
    ldmfd sp!,{fp,pc}

@op2:253.243.179
@branch 38.39.124
