#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../cpu/io.h"
#include "multiboot.h"
extern uint32_t _end_kernel;
static void* end_kernel;
static void* end_memory;
static void* first_frame;

// The possible background and forground colors
typedef enum {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} vga_color;

char* itoa(int value, char* str, int base) {
    int i = 0;
    int cbase = base;
    char digit;
    while (value > 0) {
        digit = ((value % cbase) / (cbase/base)) + 48;
        if (digit > 57){
            digit += 7;
        }
        str[i] = digit;
        value -= value % cbase;
        cbase *= base;
        i++;
    }
    char fstring[i];
    for (int j = 1; j <= i; j ++) {
        fstring[j - 1] = str[i - j];
    }
    for (int j = 0; j <= i; j++) {
        str[j] = fstring[j];
    }
    if(i == 0){
        str[0] = '0';
        i++;
    }
    str[i] = 0;// Null byte (To end string)
    return str;
}

// The great VGA number (Memory location) 0xB8000
static uint16_t* VGA_START = (uint16_t*)0xB8000;
static const size_t VGA_MAX_WIDTH = 80;
static const size_t VGA_max_HEIGHT = 25;
static size_t x = 0;
static size_t y = 0;

uint8_t get_colors(vga_color background, vga_color foreground) {
    return (background << 4) | foreground;
}

uint16_t charToEntry(char c, uint8_t color) {
    return (uint16_t)((color << 8) | c);
}

void unbuffered_scroll() {
    for (size_t cy = 0; cy < (VGA_max_HEIGHT); cy++) {
		for (size_t cx = 0; cx < VGA_MAX_WIDTH; cx++) {
			const size_t index = cy * VGA_MAX_WIDTH + cx;
			VGA_START[index] = VGA_START[index + VGA_MAX_WIDTH];
		}
	}
    for (size_t cx = 0; cx < VGA_MAX_WIDTH; cx++) {
			const size_t index = VGA_max_HEIGHT * VGA_MAX_WIDTH + cx;
			VGA_START[index] = charToEntry(' ', get_colors(VGA_COLOR_BLACK, VGA_COLOR_WHITE));
    }
}

void terminal_scroll() {
    unbuffered_scroll();
}

void moveDown() {
    if (y + 1 == VGA_max_HEIGHT){
        terminal_scroll();
    } else {
        y++;
    }
}

void moveRight() {
    if (x == VGA_MAX_WIDTH) {
        x = 0;
        moveDown();
    } else {
        x++;
    }
}

void moveUp() {
    x = VGA_MAX_WIDTH;
    if (y == 0) {
        //TODO scroll backwards
    } else {
        y--;
    }
}

void moveLeft() {
    if (x == 0) {
        moveUp();
    } else {
        x--;
    }
}

void newline() {
    x = 0;
    moveDown();
}

void update_cursor(int x, int y){
	uint16_t pos = y * VGA_MAX_WIDTH + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void putchar(char c, uint8_t colors) {
    if (c == '\n'){
        newline();
    } else {
        VGA_START[(y * VGA_MAX_WIDTH + x)] = charToEntry(c, colors);
        moveRight();
    }
    update_cursor(x, y);
}

void clear(vga_color backgroundColor) {
    uint8_t colors = get_colors(backgroundColor, VGA_COLOR_WHITE);
    for (size_t cy = 0; cy < VGA_max_HEIGHT; cy++) {
		for (size_t cx = 0; cx < VGA_MAX_WIDTH; cx++) {
			const size_t index = cy * VGA_MAX_WIDTH + cx;
			VGA_START[index] = charToEntry(' ', colors);
		}
	}
    x = 0;
    y = 0;
}

void terminal_clear() {
    clear(VGA_COLOR_BLACK);
}

void color_print(char* str, vga_color backgroundColor, vga_color foregroundColor) {
    uint8_t colors = get_colors(backgroundColor, foregroundColor);
    while(*str != '\0') {
        putchar(*str, colors);
        str++;
    }
}

void color_println(char* str, vga_color backgroundColor, vga_color foregroundColor) {
    color_print(str, backgroundColor, foregroundColor);
    putchar('\n', get_colors(backgroundColor, foregroundColor));
}

void kernel_print(char* str) {
    color_print(str, VGA_COLOR_BLACK, VGA_COLOR_WHITE);
}

void kernel_println(char* str) {
    color_println(str, VGA_COLOR_BLACK, VGA_COLOR_WHITE);
}

void kernel_error_print(char* str) {
    color_print(str, VGA_COLOR_BLACK, VGA_COLOR_LIGHT_RED);
}

void kernel_error_println(char* str) {
    color_println(str, VGA_COLOR_BLACK, VGA_COLOR_LIGHT_RED);
}


void kernel_success_print(char* str) {
    color_print(str, VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREEN);
}

void kernel_success_println(char* str) {
    color_println(str, VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREEN);
}

void kernel_backspace(){
    moveLeft();
    kernel_print(" ");
    moveLeft();
    update_cursor(x,y);
}

void kernel_main(void) {
    clear(VGA_COLOR_BLACK);
    kernel_println(" ____                        _____   ____");
    kernel_println("/\\  _`\\                     /\\  __`\\/\\  _`\\");
    kernel_println("\\ \\ \\/\\ \\    ___     ___ ___\\ \\ \\/\\ \\ \\,\\L\\_\\");
    kernel_println(" \\ \\ \\ \\ \\  / __`\\ /' __` __`\\ \\ \\ \\ \\/_\\__ \\");
    kernel_println("  \\ \\ \\_\\ \\/\\ \\L\\ \\/\\ \\/\\ \\/\\ \\ \\ \\_\\ \\/\\ \\L\\ \\" );
    kernel_println("   \\ \\____/\\ \\____/\\ \\_\\ \\_\\ \\_\\ \\_____\\ `\\____\\");
    kernel_println("    \\/___/  \\/___/  \\/_/\\/_/\\/_/\\/_____/\\/_____/");
    kernel_print("Welcome to ");
    color_print("DomOS\n", VGA_COLOR_BLACK, VGA_COLOR_LIGHT_MAGENTA);
    kernel_error_println("This is a test error print");
    int x = charToEntry('H', get_colors(VGA_COLOR_BROWN, VGA_COLOR_LIGHT_MAGENTA));
    char str[30];
    itoa(x, str, 10); 
    kernel_error_println(str);
    // asm("int $0x6");
    kernel_println("Back in Kernel");
    // kernel_println("Start Memory: ");
    // x = ((uint32_t)&_end_kernel) >> 28;
    // itoa(x, str, 16);
    // kernel_error_println(str);
    // x = ((uint32_t)&_end_kernel) & 0x0FFFFFFF;
    // itoa(x, str, 16);
    // kernel_error_println(str);
    // kernel_println("End Memory: ");
    // x = ((uint32_t)end_memory) >> 28;
    // itoa(x, str, 16);
    // kernel_error_println(str);
    // x = ((uint32_t)end_memory) & 0x0FFFFFFF;
    // itoa(x, str, 16);
    // kernel_error_println(str);
    // uint32_t* mem = (uint32_t*)&_end_kernel;
    // while((uint32_t)mem < (uint32_t)end_memory) {
    //     *mem = 0xFFFFFFFF;
    //     mem += sizeof(uint32_t);
    // }

    kernel_error_println("Setting up paging");
}


typedef struct temp_multiboot_memory_map {
	unsigned int size;
	unsigned int base_addr_low,base_addr_high;
// You can also use: unsigned long long int base_addr; if supported.
	unsigned int length_low,length_high;
// You can also use: unsigned long long int length; if supported.
	unsigned int type;
} temp_multiboot_memory_map_t;
 
// this is really an entry, not the entire map.
typedef temp_multiboot_memory_map_t temp_mmap_entry_t;


void mboot_data(multiboot_info_t* mbd) {
    // terminal_clear(VGA_COLOR_BLACK);
    // Set the end of kernel pointer (Beginning of free memory)
    end_kernel = &_end_kernel;
    first_frame = end_kernel + (4096 * 2);

    // Find the largest free entry (For now we take the largest free entry and hope it contains the kernel)
    // TODO: Check entry contains kernel and if not then designate the entire entry as free
    // TODO: Use other ram that's allegidly free
    // TODO: Figure out what's up with the randomly overlapping areas
    // TODO: Deal with RAM bigger than 4GB
    // Note: This seems to work fine with 16GB in qemu. Unsure about less standard environments.
    temp_mmap_entry_t* maxentry;
    uint32_t maxlength = 0;
    temp_mmap_entry_t* entry = (temp_mmap_entry_t*)mbd->mmap_addr;
	while((uint32_t)entry < mbd->mmap_addr + mbd->mmap_length) {
		entry = (temp_mmap_entry_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));

        if (entry->type == 1) {
            if (entry->length_high > (uint32_t)0) {
                // kernel_println("Big boi");
                if (entry->base_addr_high > (uint32_t)0) {
                    // kernel_println("Address is above 4GB");
                }
                // RAM section is larger than 4GB (And so we should just use some of it
                //(ideally 4GB of it within 32bit address space)
            }
            if (maxlength == 0 || maxlength < entry->length_low) {
                maxentry = entry;
                maxlength = entry->length_low;
            }
        }
	}
    if (maxlength > 0) {
        // char str[25];
        end_memory = (void*)(maxentry->base_addr_low + maxentry->length_low);
        // int x = ((uint32_t)maxlength) >> 28;
        // itoa(x, str, 16);
        // kernel_error_println("Max Length");
        // kernel_error_println(str);
        // x = ((uint32_t)maxlength) & 0x0FFFFFFF;
        // itoa(x, str, 16);
        // kernel_error_println(str);
    } else {
        // kernel_error_println("Well that went poorly");
    }
}