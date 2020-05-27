
#include "../kernel/kernel.h"
//Abandoned until heap.
//Later should clear terminal then print test output after running.

// char* test_outputs[256];

void assert_true(char* name, int test) {
    if(test) {
        kernel_success_println(name);
    } else {
        kernel_error_println(name);
    }
}