%include "boot/constants.asm"


isr_asm_call:
    extern isr_handler
    pusha ;Save registers

    ;; Save the current data segment
    mov ax, ds
    push eax ; Note 16 useless bits ignored
    push esp ; Point towards registers_t struct
    mov ax, KERNEL_DS ;Load Kernel Data Segments (Same as reload_CS maybe call that?)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    cld
    call isr_handler
    ; cld
    ; call isr_handler
    ; hlt
    pop ebx ; useless memory (can I pop without a dst?)
    pop ebx; Every example I've found uses eax here and ebx below. is the IRQ expected to have
    ;           a return value but here not? Using ebx in both just in case
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    mov ss, bx
    popa
    add esp, 8
    iret


irq_asm_call:
    extern irq_handler
    pusha ;Save registers

    ;; Save the current data segment
    mov ax, ds
    push eax ; Note 16 useless bits ignored
    push esp ; Point towards registers_t struct
    mov ax, KERNEL_DS ;Load Kernel Data Segments (Same as reload_CS maybe call that?)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    cld
    call irq_handler
    ; cld
    ; call isr_handler
    ; hlt
    pop ebx ; useless memory (can I pop without a dst?)
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    mov ss, bx
    popa
    add esp, 8
    iret


global isr_0
isr_0:
    push 0
    push 0
    jmp isr_asm_call


global isr_1
isr_1:
    push 0
    push 1
    jmp isr_asm_call


global isr_2
isr_2:
    push 0
    push 2
    jmp isr_asm_call


global isr_3
isr_3:
    push 0
    push 3
    jmp isr_asm_call

global isr_4
isr_4:
    push 0
    push 4
    jmp isr_asm_call

global isr_5
isr_5:
    push 0
    push 5
    jmp isr_asm_call

global isr_6
isr_6:
    push 0
    push 6
    jmp isr_asm_call

global isr_7
isr_7:
    push 0
    push 7
    jmp isr_asm_call

global isr_8
isr_8:
    push 8
    jmp isr_asm_call

global isr_9
isr_9:
    push 0
    push 9
    jmp isr_asm_call

global isr_10
isr_10:
    push 10
    jmp isr_asm_call

global isr_11
isr_11:
    push 11
    jmp isr_asm_call

global isr_12
isr_12:
    push 12
    jmp isr_asm_call

global isr_13
isr_13:
    push 13
    jmp isr_asm_call

global isr_14
isr_14:
    push 14
    jmp isr_asm_call


global isr_15
isr_15:
    push 0
    push 15
    jmp isr_asm_call


global isr_16
isr_16:
    push 0
    push 16
    jmp isr_asm_call

global isr_17
isr_17:
    push 17
    jmp isr_asm_call

global isr_18
isr_18:
    push 0
    push 18
    jmp isr_asm_call

global isr_19
isr_19:
    push 0
    push 19
    jmp isr_asm_call


global isr_20
isr_20:
    push 0
    push 20
    jmp isr_asm_call

global isr_21
isr_21:
    push 0
    push 21
    jmp isr_asm_call

global isr_22
isr_22:
    push 0
    push 22
    jmp isr_asm_call

global isr_23
isr_23:
    push 0
    push 23
    jmp isr_asm_call

global isr_24
isr_24:
    push 0
    push 24
    jmp isr_asm_call

global isr_25
isr_25:
    push 0
    push 25
    jmp isr_asm_call

global isr_26
isr_26:
    push 0
    push 26
    jmp isr_asm_call

global isr_27
isr_27:
    push 0
    push 27
    jmp isr_asm_call

global isr_28
isr_28:
    push 0
    push 28
    jmp isr_asm_call

global isr_29
isr_29:
    push 0
    push 29
    jmp isr_asm_call

global isr_30
isr_30:
    push 30
    jmp isr_asm_call

global isr_31
isr_31:
    push 0
    push 31
    jmp isr_asm_call


global irq_0
irq_0:
    push 0
    push 0
    jmp irq_asm_call

global irq_1
irq_1:
    push 0
    push 1
    jmp irq_asm_call

global irq_2
irq_2:;Should never get called
    push 0
    push 2
    jmp irq_asm_call


global irq_3
irq_3:
    push 0
    push 3
    jmp irq_asm_call

global irq_4
irq_4:
    push 0
    push 4
    jmp irq_asm_call

global irq_5
irq_5:
    push 0
    push 5
    jmp irq_asm_call

global irq_6
irq_6:
    push 0
    push 6
    jmp irq_asm_call

global irq_7
irq_7:
    push 0
    push 7
    jmp irq_asm_call


global irq_8
irq_8:
    push 0
    push 8
    jmp irq_asm_call


global irq_9
irq_9:
    push 0
    push 9
    jmp irq_asm_call


global irq_10
irq_10:
    push 0
    push 10
    jmp irq_asm_call


global irq_11
irq_11:
    push 0
    push 11
    jmp irq_asm_call

global irq_12
irq_12:
    push 0
    push 12
    jmp irq_asm_call

global irq_13
irq_13:
    push 0
    push 13
    jmp irq_asm_call

global irq_14
irq_14:
    push 0
    push 14
    jmp irq_asm_call

global irq_15
irq_15:
    push 0
    push 15
    jmp irq_asm_call
