#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "kernel.h"

static uint32_t* page_directory;
static uint32_t* kernel_page_tables;

void setup_paging(void* page_directory_in, void* kernel_start, void* kernel_end) {
    char str[25];
    kernel_println("Setting up paging");
    page_directory = (uint32_t*)page_directory_in;
    for(int i = 0; i < 1024; i++)
    {
        page_directory[i] = 0x00000002;//Kernel only and not present
    }
    uint32_t* first_page_table = &page_directory[1024]; // Put the kernel page tables after the kernel page directory
    // Later along we'll put page tables in virtual memory but we need a page table prepared before
    // enabling paging/virtual memory
    page_directory[0] = ((uint32_t)first_page_table) | 0b11;

    // Point 0xC0100000 to the kernel
    page_directory[768] = ((uint32_t)first_page_table) | 0b11;//Readable, writable and Present
    void* current_page = kernel_start;// Doesn't allow for multiple PD entries for now
    for(int i = 0; i < 1024; i++)
    {
        first_page_table[i] = 0x00000002;//Kernel only and not present
    }
    int i = 256;
    kernel_print("Expected Kernel pages Created: ");
    itoa(((uint32_t)kernel_end - (uint32_t)current_page)/4096, str, 10);
    kernel_println(str);
    while ((uint32_t)current_page < (uint32_t)kernel_end) {
        first_page_table[i] = ((uint32_t)current_page) | 0b11;//Readable, writable and Present
        current_page += 4096;
        i++;
    }
    first_page_table[184] = ((uint32_t)0xB8000) | 0b11;
    first_page_table[185] = ((uint32_t)0xB9000) | 0b11;
    kernel_print("Kernel pages created: ");
    itoa(i - 256, str, 10);
    kernel_println(str);
    kernel_println("Done Building Tables");
}