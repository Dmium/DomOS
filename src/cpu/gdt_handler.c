//TODO Damn this file is a mess. Just go back to manually calculating the entries

#include <stddef.h>
#include <stdint.h>
// Used for creating GDT segment descriptors in 64-bit integer form.

#include <stdint.h>
 
// Each define here is for a specific flag in the descriptor.
// Refer to the intel documentation for a description of what each one does.
#define SEG_DESCTYPE(x)  ((x) << 0x04) // Descriptor type (0 for system, 1 for code/data)
#define SEG_PRES(x)      ((x) << 0x07) // Present
#define SEG_SAVL(x)      ((x) << 0x0C) // Available for system use
#define SEG_LONG(x)      ((x) << 0x0D) // Long mode
#define SEG_SIZE(x)      ((x) << 0x0E) // Size (0 for 16-bit, 1 for 32)
#define SEG_GRAN(x)      ((x) << 0x0F) // Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
#define SEG_PRIV(x)     (((x) &  0x03) << 0x05)   // Set privilege level (0 - 3)
 
#define SEG_DATA_RD        0x00 // Read-Only
#define SEG_DATA_RDA       0x01 // Read-Only, accessed
#define SEG_DATA_RDWR      0x02 // Read/Write
#define SEG_DATA_RDWRA     0x03 // Read/Write, accessed
#define SEG_DATA_RDEXPD    0x04 // Read-Only, expand-down
#define SEG_DATA_RDEXPDA   0x05 // Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD  0x06 // Read/Write, expand-down
#define SEG_DATA_RDWREXPDA 0x07 // Read/Write, expand-down, accessed
#define SEG_CODE_EX        0x08 // Execute-Only
#define SEG_CODE_EXA       0x09 // Execute-Only, accessed
#define SEG_CODE_EXRD      0x0A // Execute/Read
#define SEG_CODE_EXRDA     0x0B // Execute/Read, accessed
#define SEG_CODE_EXC       0x0C // Execute-Only, conforming
#define SEG_CODE_EXCA      0x0D // Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC     0x0E // Execute/Read, conforming
#define SEG_CODE_EXRDCA    0x0F // Execute/Read, conforming, accessed
 
#define GDT_CODE_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_CODE_EXRD
 
#define GDT_DATA_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_DATA_RDWR
 
#define GDT_CODE_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_CODE_EXRD
 
#define GDT_DATA_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_DATA_RDWR
 
uint64_t create_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    uint64_t descriptor;
 
    // Create the high 32 bit segment
    descriptor  =  limit       & 0x000F0000;         // set limit bits 19:16
    descriptor |= (flag <<  8) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
    descriptor |= (base >> 16) & 0x000000FF;         // set base bits 23:16
    descriptor |=  base        & 0xFF000000;         // set base bits 31:24
 
    // Shift by 32 to allow for low part of segment
    descriptor <<= 32;
 
    // Create the low 32 bit segment
    descriptor |= base  << 16;                       // set base bits 15:0
    descriptor |= limit  & 0x0000FFFF;               // set limit bits 15:0
    return descriptor;
}
 
void gdt_setup(uint64_t* nullentry, uint64_t* codeentry, uint64_t* dataentry, uint64_t* usercodeentry, uint64_t* userdataentry)
{
    *(nullentry) = create_descriptor(0, 0, 0);
    *(codeentry) = create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0));
    *(dataentry) = create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0));
    *(usercodeentry) = create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3));
    *(userdataentry) = create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3));
}





















// void encodeGdtEntry(uint8_t *target, struct GDT source)
// {
//     // Check the limit to make sure that it can be encoded
//     if ((source.limit > 65536) && ((source.limit & 0xFFF) != 0xFFF)) {
//         kerror("You can't do that!");
//     }
//     if (source.limit > 65536) {
//         // Adjust granularity if required
//         source.limit = source.limit >> 12;
//         target[6] = 0xC0;
//     } else {
//         target[6] = 0x40;
//     }
 
//     // Encode the limit
//     target[0] = source.limit & 0xFF;
//     target[1] = (source.limit >> 8) & 0xFF;
//     target[6] |= (source.limit >> 16) & 0xF;
 
//     // Encode the base 
//     target[2] = source.base & 0xFF;
//     target[3] = (source.base >> 8) & 0xFF;
//     target[4] = (source.base >> 16) & 0xFF;
//     target[7] = (source.base >> 24) & 0xFF;
 
//     // And... Type
//     target[5] = source.type;
// }








// #include <stddef.h>
// #include <stdint.h>
// // #include <memstuff.h>
// struct _gdt_entry_bits{
// 	unsigned int limit_low:16;
// 	unsigned int base_low : 24;
//      //attribute byte split into bitfields
// 	unsigned int accessed :1;
// 	unsigned int read_write :1; //readable for code, writable for data
// 	unsigned int direction_confirming :1; //conforming for code, direction for data
// 	unsigned int executable :1; //1 for code, 0 for data
// 	unsigned int descripter_type :1;
// 	unsigned int priv :2; //priviledge level
// 	unsigned int present :1;
//      //and now into granularity
// 	unsigned int limit_high :4;
// 	unsigned int available :1;
// 	unsigned int always_0 :1; //should always be 0
// 	unsigned int size :1; //32bit opcodes for code, uint32_t stack for data
// 	unsigned int gran :1; //1 to use 4k page addressing, 0 for byte addressing
// 	unsigned int base_high :8;
// }  __packed;
// typedef struct _gdt_entry_bits gdt_entry_bits;




// void gdt_setup(uint64_t* nullentry, gdt_entry_bits* codeentry, gdt_entry_bits* dataentry) {
//     *nullentry = (uint64_t)0;
    
//     codeentry->limit_low = 0xFFFF;
//     codeentry->limit_high = 0xF;
//     codeentry->base_low = 0;
//     codeentry->base_high = 0;

//     //Access Byte
//     codeentry->accessed = 0;
//     codeentry->read_write = 1;
//     codeentry->direction_confirming = 0;
//     codeentry->executable = 1;
//     codeentry->descripter_type = 1;
//     codeentry->priv = 0;//ring0
//     codeentry->present = 1;

//     //flags
//     codeentry->gran=1;
//     codeentry->size = 1;
    
//     codeentry->always_0 = 0;
//     codeentry->available = 1;

//     *dataentry = *codeentry;
//     dataentry->executable = 0;

//     // dataentry->limit_low = (uint16_t)0xFFFF;
//     // dataentry->limit_high = 0xF;
//     // dataentry->base_low = 0;
//     // dataentry->base_high = 0;

//     // //Access Byte
//     // dataentry->accessed = 0;
//     // dataentry->read_write = 1;
//     // dataentry->direction_confirming = 0;
//     // dataentry->executable = 0;
//     // dataentry->descripter_type = 1;
//     // dataentry->priv = 0;//ring0
//     // dataentry->present = 1;

//     // //flags
//     // dataentry->gran=1;
//     // dataentry->size = 1;


//     // dataentry->always_0 = 0;
//     // dataentry->available = 0;
// }