#include <stddef.h>
#include <stdint.h>

#include "string.h"
#include "kernel.h"

static char input_buffer[2048];
static int writer = 0;
// char* reader = input_buffer;

void parse_and_execute() {
    kernel_println("");
    if (strcmp(input_buffer, "hi")) {
        kernel_println("Hello");
    } else {
        kernel_println("Unknown Command");
    }
    kernel_print(">");
}

void putc(char c) {
    if (c == '\n') {
        input_buffer[writer] = 0;
        writer = 0;
        parse_and_execute();
    } else {
        char str[2] = {c, 0};
        kernel_print(str);
        input_buffer[writer] = c;
        writer++;
        // input_buffer[writer++] = 0;
        // kernel_println(input_buffer);
    }
}


// char getchar() {
//     char c = *reader;
//     reader++;
//     return c;
// }
