#include <stddef.h>
#include "kernel.h"
size_t strlen(char* str) {
    size_t len = 0;
    while(str[len] != 0) {
        len++;
    }
    return len;
}

//TODO proper return values (currently just true or false)
int strcmp(char* str1, char* str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t pos = 0;
    if (len1 == len2) {
        while(pos < len1 && str1[pos] == str2[pos]) {
            pos++;
        }
        return (len1 == pos);
    }
    return 0;

}

// void strcat(char* str1, char* str2) {
// }