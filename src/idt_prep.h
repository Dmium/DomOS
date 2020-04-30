#ifndef IDT_PREP_H
#define IDT_PREP_H
#include <stddef.h>
#include <stdint.h>


extern void isr_0();
extern void isr_1();
extern void isr_2();
extern void isr_3();
extern void isr_4();
extern void isr_5();
extern void isr_6();
extern void isr_7();
extern void isr_8();
extern void isr_9();
extern void isr_10();
extern void isr_11();
extern void isr_12();
extern void isr_13();
extern void isr_14();
extern void isr_15();
extern void isr_16();
extern void isr_17();
extern void isr_18();
extern void isr_19();
extern void isr_20();
extern void isr_21();
extern void isr_22();
extern void isr_23();
extern void isr_24();
extern void isr_25();
extern void isr_26();
extern void isr_27();
extern void isr_28();
extern void isr_29();
extern void isr_30();
extern void isr_31();
extern void irq_0();
extern void irq_1();
extern void irq_2();
extern void irq_3();
extern void irq_4();
extern void irq_5();
extern void irq_6();
extern void irq_7();
extern void irq_8();
extern void irq_9();
extern void irq_10();
extern void irq_11();
extern void irq_12();
extern void irq_13();
extern void irq_14();
extern void irq_15();
typedef struct {
   uint16_t offset_1; // offset bits 15-0
   uint16_t selector; // a code segment selector in GDT or LDT
   uint8_t zero;      // unused, set to 0
   uint8_t type_attr; // type and attributes, see below
   uint16_t offset_2; // offset bits 31-16
} IDTDescr;


typedef struct {
   uint32_t ds; // Data segment
   uint32_t edi, esi, ebp, useless_value, ebx, edx, ecx, eax; //Saved regs
   uint32_t interruptID, err_code;//err_code is 0 if no error
   uint32_t eip, cs, eflags, esp, ss;
} registers_t; // Aligned so no packing needed

void initIDT(IDTDescr*);
void irq_handler(registers_t*);
void isr_handler(registers_t*);

#endif