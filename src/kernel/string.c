#include <stddef.h>

size_t strlen(char* str) {
    size_t len = 0;
    while(str[len] != 0) {
        len++;
    }
    return len;
}

//TODO proper return values (currently just true or false)
int strcmp(char* str1, char* str2) {
    size_t len = strlen(str1);
    size_t pos = 0;
    while(str1[pos] == str2[pos]) {
        pos++;
    }
    return (len + 1 == pos);

}