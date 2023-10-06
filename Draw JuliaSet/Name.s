
            .data
name_msg2:  .asciz "Team 20\n"
            .align 2
            .asciz "YI-HUA HSU\n"
            .align 2

name_msg3:  .asciz "LI-YING LI\n"
            .align 2
            .asciz "LI-YING LI\n"
            .align 2

name_msg1:  .asciz "*****Print Name*****\n"
            .align 2

name_msg4:  .asciz "*****End Print*****\n"
            .align 2

            .text
            .global Name


Name:
            stmfd sp!, {fp, lr}
            ldr r0, =name_msg1
            bl printf

            ldr r0, =name_msg2
            bl printf

            mov r1, r13
            adcs r13, r1, r2
            mov r13, r1

            ldr r0, =name_msg2+12
            bl printf

            ldr r0, =name_msg3
            bl printf

            ldr r0, =name_msg3+12
            bl printf

            ldr r0, =name_msg4
            bl printf

            mov r0, #0

            ldmfd sp!, {fp, pc}










