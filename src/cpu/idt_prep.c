#include <stddef.h>
#include <stdint.h>
#include "idt_prep.h"
#include "io.h"

#include "../kernel/kernel.h"





void prepareEntry(IDTDescr* entry, uint32_t offset) {//IDTEntry* settings) {
    //These casts are probably uneeded but eh
    // entry->offset_1 = (uint16_t)(settings->offset && 0xFFFF);
    entry->offset_1 = (uint16_t)(offset & 0xFFFF);
    entry->selector = (uint16_t)0x0008;
    entry->zero = (uint8_t)0x00;
    entry->type_attr = (uint8_t)0x8F;//Temporarily using the same for all
    // entry->offset_2 = (uint16_t)((settings->offset >> 16) && 0xFFFF);
    entry->offset_2 = (uint16_t)((offset >> 16) & 0xFFFF);
}
// Stolen from the OSDEV wiki (Because it's the same everywhere)
void PIC_remap(int offset1, int offset2)
{
	unsigned char a1, a2;
 
	a1 = inb(PIC1_DATA);                        // save masks
	a2 = inb(PIC2_DATA);
 
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
 
	outb(PIC1_DATA, a1);   // restore saved masks.
	outb(PIC2_DATA, a2);
}

void initIDT(IDTDescr* table) {
    prepareEntry(&(table[0]), (0xC0000000) + (uint32_t)isr_0);
    prepareEntry(&(table[1]), (0xC0000000) + (uint32_t)isr_1);
    prepareEntry(&(table[2]), (0xC0000000) + (uint32_t)isr_2);
    prepareEntry(&(table[3]), (0xC0000000) + (uint32_t)isr_3);
    prepareEntry(&(table[4]), (0xC0000000) + (uint32_t)isr_4);
    prepareEntry(&(table[5]), (0xC0000000) + (uint32_t)isr_5);
    prepareEntry(&(table[6]), (0xC0000000) + (uint32_t)isr_6);
    prepareEntry(&(table[7]), (0xC0000000) + (uint32_t)isr_7);
    prepareEntry(&(table[8]), (0xC0000000) + (uint32_t)isr_8);
    prepareEntry(&(table[9]), (0xC0000000) + (uint32_t)isr_9);
    prepareEntry(&(table[10]), (0xC0000000) + (uint32_t)isr_10);
    prepareEntry(&(table[11]), (0xC0000000) + (uint32_t)isr_11);
    prepareEntry(&(table[12]), (0xC0000000) + (uint32_t)isr_12);
    prepareEntry(&(table[13]), (0xC0000000) + (uint32_t)isr_13);
    prepareEntry(&(table[14]), (0xC0000000) + (uint32_t)isr_14);
    prepareEntry(&(table[15]), (0xC0000000) + (uint32_t)isr_15);
    prepareEntry(&(table[16]), (0xC0000000) + (uint32_t)isr_16);
    prepareEntry(&(table[17]), (0xC0000000) + (uint32_t)isr_17);
    prepareEntry(&(table[18]), (0xC0000000) + (uint32_t)isr_18);
    prepareEntry(&(table[19]), (0xC0000000) + (uint32_t)isr_19);
    prepareEntry(&(table[20]), (0xC0000000) + (uint32_t)isr_20);
    prepareEntry(&(table[21]), (0xC0000000) + (uint32_t)isr_21);
    prepareEntry(&(table[22]), (0xC0000000) + (uint32_t)isr_22);
    prepareEntry(&(table[23]), (0xC0000000) + (uint32_t)isr_23);
    prepareEntry(&(table[24]), (0xC0000000) + (uint32_t)isr_24);
    prepareEntry(&(table[25]), (0xC0000000) + (uint32_t)isr_25);
    prepareEntry(&(table[26]), (0xC0000000) + (uint32_t)isr_26);
    prepareEntry(&(table[27]), (0xC0000000) + (uint32_t)isr_27);
    prepareEntry(&(table[28]), (0xC0000000) + (uint32_t)isr_28);
    prepareEntry(&(table[29]), (0xC0000000) + (uint32_t)isr_29);
    prepareEntry(&(table[30]), (0xC0000000) + (uint32_t)isr_30);
    prepareEntry(&(table[31]), (0xC0000000) + (uint32_t)isr_31);
    prepareEntry(&(table[32]), (0xC0000000) + (uint32_t)irq_0);
    prepareEntry(&(table[33]), (0xC0000000) + (uint32_t)irq_1);
    prepareEntry(&(table[34]), (0xC0000000) + (uint32_t)irq_2);
    prepareEntry(&(table[35]), (0xC0000000) + (uint32_t)irq_3);
    prepareEntry(&(table[36]), (0xC0000000) + (uint32_t)irq_4);
    prepareEntry(&(table[37]), (0xC0000000) + (uint32_t)irq_5);
    prepareEntry(&(table[38]), (0xC0000000) + (uint32_t)irq_6);
    prepareEntry(&(table[39]), (0xC0000000) + (uint32_t)irq_7);
    prepareEntry(&(table[40]), (0xC0000000) + (uint32_t)irq_8);
    prepareEntry(&(table[41]), (0xC0000000) + (uint32_t)irq_9);
    prepareEntry(&(table[42]), (0xC0000000) + (uint32_t)irq_10);
    prepareEntry(&(table[43]), (0xC0000000) + (uint32_t)irq_11);
    prepareEntry(&(table[44]), (0xC0000000) + (uint32_t)irq_12);
    prepareEntry(&(table[45]), (0xC0000000) + (uint32_t)irq_13);
    prepareEntry(&(table[46]), (0xC0000000) + (uint32_t)irq_14);
    prepareEntry(&(table[47]), (0xC0000000) + (uint32_t)irq_15);
    PIC_remap((int)0x20, (int)0x28);
}