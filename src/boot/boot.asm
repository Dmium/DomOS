; A20 Set by GRUB (Do I test anyway? idek how I would know I'm successfully enabling it without a test env)
%include "boot/constants.asm"
; %include "kernel/interrupt.asm"
; KERNEL_CS           equ 0x08
; KERNEL_DS           equ 0x10


MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
; pass us a symbol table.
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


[BITS 32]                       ; All instructions should be 32-bit.
section .multiboot

multiboot:
align 4
  dd  MBOOT_HEADER_MAGIC
  dd  MBOOT_HEADER_FLAGS
  dd  MBOOT_CHECKSUM


section .bss
align 16
stack_bottom: ; I'm not convinced this does anything
resb 16384 ; 16 KiB
stack_top:
; align 4096
gdt_null:
resb 8
gdt_code:
resb 8
gdt_data:
resb 8
gdt_user_code:
resb 8
gdt_user_data:
resb 8
gdt_end:
; align 4096 ;Probs a waste of data but I'm not writing this to be RAM efficient
idt_start:
resb 8*47 ;TODO change to 64 (8*64=256 entries)
idt_end:




section .data
align 4096
;TODO Should I put the descriptors in .bss and set them .text? May make paging easier
gdt_desc:
    dw gdt_end - gdt_null ;for size
    dd gdt_null ;for offset

idtr_prep:
    dw idt_end - idt_start - 1; Limit ie size of the table (-1 TODO did I do the maths right?)
    dd idt_start; Base (Points to the start of the idt)

section .text

[GLOBAL _start]
_start:
    mov esp, stack_top ;Nice to have a stack pointer point at the stack
    ; push ebx
    ; push eax
    extern mboot_data
    push ebx
    call mboot_data
    extern kernel_main
    extern gdt_setup
    extern initIDT

    ;;GDT Stuff
    cli ; No Interrupts during gdt things
    ;push gdt pointers to stack
    push gdt_user_data
    push gdt_user_code
    push gdt_data
    push gdt_code
    push gdt_null
    call gdt_setup ; Setup GDT (duh)

    lgdt [gdt_desc]; Load our(painfully) setup gdt

    call reload_segments


    ;;IDT Stuff
    push idt_start
    call initIDT
    lidt [idtr_prep]
    sti ; Resume
    call kernel_main
    ; push 5
    ; push 5
    ; push 5
    ; int 0xE
    int 0x3
    ;   call kernel_main
    ; cli
    jmp halt
halt:
    ; cli
    hlt
    jmp halt ;Infinite loop after kernel run
reload_segments:
    jmp KERNEL_CS:reload_CS; 0x08 points at the new code selector cs
    ; We use a jmp here because we can't do 'mov cs, 0x08' and the jmp does it for us

reload_CS:
   ; Reload data segment registers
    mov ax, KERNEL_DS ; 0x10 points at the new data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret