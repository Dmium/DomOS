#include <stddef.h>
#include <stdint.h>
#include "kernel.h"
#include "../cpu/idt_prep.h"
#include "../cpu/io.h"
#include "../drivers/keyboard.h"

// TODO Cleanup and handle nicer.
// static int timer = 0;
void irq_handler(registers_t* regs){
    char idstr[13];
    if (regs->interruptID >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
 
        outb(PIC1_COMMAND, PIC_EOI);
        kernel_println("Slave IRQ");
        itoa(regs->interruptID, idstr, 10);
        kernel_println(idstr);
    }
    else if (regs->interruptID == 0) {
        // timer++;
        // if (timer % 30 == 0) {
        //     kernel_println("Interrupt");
        //     itoa(regs->interruptID, idstr, 10);
        //     kernel_println(idstr);
        // }
 
        outb(PIC1_COMMAND, PIC_EOI);
    }
    else if (regs->interruptID == 1) {
        on_keyboard_interrupt();
        // uint8_t sc = 0;
        // sc = inb(0x60);// Get scancode
        // kernel_println("Keyboard");
        // itoa(sc, idstr, 16);
        // kernel_println(idstr);
        // if(sc == 0x91) {
        //     kernel_println("Wup");
        // }
        // outb(PIC1_COMMAND, PIC_EOI);
    }
    else if (regs->interruptID <= 7) {
        outb(PIC1_COMMAND, PIC_EOI);
        kernel_println("Master IRQ");
        itoa(regs->interruptID, idstr, 10);
        kernel_println(idstr);
    }
}
void isr_handler(registers_t* regs){
    char idstr[100];
    if(regs->interruptID <= 31){
        // kernel_println("Interrupt");
        itoa(regs->interruptID, idstr, 10);
        // kernel_println(idstr);
        if (regs->interruptID == 14) {
            itoa(regs->err_code, idstr, 10);
            kernel_println(idstr);
        }
    }
}