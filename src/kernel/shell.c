#include <stddef.h>
#include <stdint.h>

#include "string.h"
#include "kernel.h"
#include "tests.h"

static char input_buffer[2048];
static int writer = 0;
// char* reader = input_buffer;

void parse_and_execute() {
    kernel_println("");
    if (strcmp(input_buffer, "hi")) {
        kernel_println("Hello");
    } else if (strcmp(input_buffer, "help")) {
        kernel_println("Help");
        kernel_println("--------------------");
        kernel_println("Available Commands:");
        kernel_println("-help");
        kernel_println("-hi");
        kernel_println("-test");
        kernel_println("--------------------");
    } else if (strcmp(input_buffer, "test")) {
        test_all();
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
    } else if (c == '\b'){
        if (writer > 0) {
            writer--;
            input_buffer[writer] = 0;
            kernel_backspace();
        }
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
