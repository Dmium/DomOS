#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
    for (size_t cy = 0; cy < (VGA_max_HEIGHT - 1); cy++) {
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
    if (y == VGA_max_HEIGHT){
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
void newline() {
    x = 0;
    moveDown();
}
void putchar(char c, uint8_t colors) {
    if (c == '\n'){
        newline();
    } else {
        VGA_START[(y * VGA_MAX_WIDTH + x)] = charToEntry(c, colors);
        x++;
    }
}

void clear(vga_color backgroundColor) {
    uint8_t colors = get_colors(backgroundColor, VGA_COLOR_WHITE);
    for (size_t cy = 0; cy < VGA_max_HEIGHT; cy++) {
		for (size_t cx = 0; cx < VGA_MAX_WIDTH; cx++) {
			const size_t index = cy * VGA_MAX_WIDTH + cx;
			VGA_START[index] = charToEntry(' ', colors);
		}
	}
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
    color_println(str, VGA_COLOR_BLACK, VGA_COLOR_LIGHT_RED);
}

void kernel_error_println(char* str) {
    color_println(str, VGA_COLOR_BLACK, VGA_COLOR_LIGHT_RED);
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
}