#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
// The great VGA formula: (y * 80 + x) * 2
static uint16_t* VGA_START = (uint16_t*)0xB8000;
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static size_t x = 0;
static size_t y = 0;

uint16_t charToEntry(char c, uint8_t color) {
    return (uint16_t)((color << 8) | c);
}

void unbuffered_scroll() {
    for (size_t cy = 0; cy < (VGA_HEIGHT - 1); cy++) {
		for (size_t cx = 0; cx < VGA_WIDTH; cx++) {
			const size_t index = cy * VGA_WIDTH + cx;
			VGA_START[index] = VGA_START[index + VGA_WIDTH];
		}
	}
    for (size_t cx = 0; cx < VGA_WIDTH; cx++) {
			const size_t index = VGA_HEIGHT * VGA_WIDTH + cx;
			VGA_START[index] = charToEntry(' ', (uint8_t)0x0f);
    }
}

void terminal_scroll() {
    unbuffered_scroll();
}

void moveDown() {
    if (y == VGA_HEIGHT){
        terminal_scroll();
    } else {
        y++;
    }
}

void moveRight() {
    if (x == VGA_WIDTH) {
        x = 0;
        moveDown();
    } else {
        x++;
    }
}

void putchar(char toPut) {
    VGA_START[(y * VGA_WIDTH + x)] = charToEntry(toPut,(uint8_t)0x0f);
    x++;
}

void clear() {
    for (size_t cy = 0; cy < VGA_HEIGHT; cy++) {
		for (size_t cx = 0; cx < VGA_WIDTH; cx++) {
			const size_t index = cy * VGA_WIDTH + cx;
			VGA_START[index] = charToEntry(' ',(uint8_t)0x0f);
		}
	}
}

void kernel_main(void) {
    clear();
    putchar('H');
    putchar('e');
    putchar('l');
    putchar('l');
    putchar('o');
    putchar('!');
    return;
}