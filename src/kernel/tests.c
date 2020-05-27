#include "kernel.h"
#include "../tests/assertions.h"
#include "string.h"
void test_tests() {
    kernel_println("Testing Tests");
    assert_true("Test passes", 1 == 1);
    assert_true("Test should fail", 1 == 0);
    kernel_println("----------------------------------------");
}

void test_strings() {
    kernel_println("Testing Strings");
    assert_true("strlen gets correct length", strlen("some characters") == (size_t)15);
    assert_true("strcmp finds two strings equivilent", strcmp("five", "five"));
    assert_true("strcmp finds two strings to be different", !strcmp("5", "five"));
    kernel_println("----------------------------------------");
}

void test_itoa() {
    kernel_println("Testing itoa");
    char itoaoutput[20];
    itoa(0, itoaoutput, 10);
    assert_true("itoa terminates", 1);
    assert_true("itoa can translate '0'", strcmp(itoaoutput, "0"));
    itoa(156, itoaoutput, 10);
    assert_true("itoa can translate '156' with base 10", strcmp(itoaoutput, "156"));
    itoa(0x1F43, itoaoutput, 16);
    assert_true("itoa can translate '0x1F43' with base 16", strcmp(itoaoutput, "1F43"));
    itoa(0b1001, itoaoutput, 2);
    assert_true("itoa can translate '0b1001' with base 2", strcmp(itoaoutput, "1001"));
    kernel_println("----------------------------------------");
}

void test_all() {
    terminal_clear();
    test_tests();
    test_strings();
    test_itoa();
}