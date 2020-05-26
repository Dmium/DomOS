#include <stddef.h>
#include <stdint.h>
#include "../kernel/kernel.h"
#include "../kernel/shell.h"
#include "../cpu/io.h"
#include "keyboard.h"

void on_keyboard_interrupt(){
    // char idstr[13];
    uint8_t sc = 0;
    sc = inb(0x60); // Get scancode
    if (sc < 128){
        putc(US_KEYBOARD[sc]);
        // idstr[0] = US_KEYBOARD[sc];

        // idstr[1] = 0;
        // kernel_print(idstr);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}