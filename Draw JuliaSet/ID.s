    .data

n:      .word 0
        .align 2
        .word 0
        .align 2
        .word 0
        .align 2
        .word 0
        .align 2

str1:   .asciz "%d"


str2:
        .asciz "%s"
        .align 2


str3:
        .asciz "%d\n"


str4:
        .asciz "p"
        .align 2

str5:
        .asciz ""
        .align 2

str6:
        .word 0
        .align 2

id_info:
    .asciz "*****Input ID*****\n"
    .align 2

id_info1:
    .asciz "**Please Enter Member 1 ID:**\n"
    .align 2

id_info2:
    .asciz "**Please Enter Member 2 ID:**\n"
    .align 2

id_info3:
    .asciz "**Please Enter Member 3 ID:**\n"
    .align 2
    .asciz "** Please Enter Command **\n"
    .align 2
    .asciz "*****Print Team Member ID and ID Summation*****\n"
    .align 2

id_info4:
    .asciz "\nID Summation = "
    .align 2
    .asciz "*****End Print*****\n\n"
    .align 2

id_info5:
    .asciz "Input error!!\n"
    .align 2

id_info6:
    .asciz "Correct command\n"
    .align 2

    .text
    .global id

id:
    stmfd sp!,{fp,lr,r0-r3}

    ldr r0, =id_info
    bl printf

    @ first num
    ldr r0, =id_info1
    bl printf

    ldr r0, =str1
    ldr r1, =n
    bl scanf

    @ second num
    ldr r0, =id_info2
    bl printf

    ldr r0, =str1
    ldr r1, =n+4
    bl scanf

    @ third num

    ldr r0, =id_info3
    bl printf

    ldr r0, =str1
    ldr r1, =n+8
    bl scanf

    loop_a:
        @ Enter command
        ldr r0, =id_info3+32
        bl printf

        ldr r0, =str2
        ldr r1, =str6
        bl scanf
        @ scanf 'p'

        ldr r8, =str4 @r8 == 'p'
        ldr r8, [r8, #0]
        ldr r1, =str6
        ldr r1, [r1]

        cmp r1, r8
        ldrne r0, =id_info5
        ldreq r0, =id_info6
        bl printf

        ldr r1, =str6
        ldr r1, [r1]
        cmp r1, r8
        beq done
        blne loop_a

done:

    addcc r0, r1, r2

    ldr r0, =id_info3
    add r0, r0, #60
    bl printf

    @ print three nums
    ldr r4, =n
    ldr r5, =n+4
    ldr r6, =n+8

    ldr r0, =str3
    ldr r1, [r4, #0]!
    bl printf

    ldr r0, =str3
    ldr r1, [r5]
    bl printf

    ldr r0, =str3
    ldr r1, [r6]
    bl printf

    ldr r0, =id_info4
    bl printf

    @ add three num
    ldr r4, =n
    ldr r4, [r4]
    ldr r5, =n+4
    ldr r5, [r5]
    ldr r6, =n+8
    ldr r6, [r6]

    add r7, r4, r5
    add r7, r7, r6, lsl#0

    ldr r0, =n+12
    str r7, [r0]

    ldr r0, =str3
    mov r1, r7
    bl printf

    ldr r0, =id_info4+20
    bl printf

    ldmfd sp!, {r0-r3}

    ldr r4, =n
    ldr r4, [r4]
    str r4, [r0]
    ldr r4, =n+4
    ldr r4, [r4]
    str r4, [r1]
    ldr r4, =n+8
    ldr r4, [r4]
    str r4, [r2]
    ldr r4, =n+12
    ldr r4, [r4]
    str r4, [r3]


    ldmfd sp!,{fp, pc}

