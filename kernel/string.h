#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

bool isalpha(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

void* memcpy( void* dest, const void* src, size_t count )
{
    unsigned char *d = (unsigned char*) dest;
    unsigned char *s = (unsigned char*) src;
    for(size_t i = 0; i < count; i++) {
        *(d + i) = *(s + i);
    }

    return dest;
}

int strncmp(const char *str1, const char *str2, size_t n)
{
    for(unsigned int i = 0; i < n; i++) {
        char a = str1[i];
        char b = str2[i];
        if(a != b) {
            return a < b ? -1 : 1;
        }
    }
    return 0;
}
// Function to reverse a string
void reverse(char* str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char* itoa_decimal(int value, char* buffer) {
    int i = 0;
    int is_negative = 0;

    // Handle 0
    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return buffer;
    }

    // Handle negative numbers
    if (value < 0) {
        is_negative = 1;
        value = -value; // Convert to positive for digit extraction
    }

    // Extract digits
    while (value != 0) {
        int remainder = value % 10;
        buffer[i++] = remainder + '0';
        value = value / 10;
    }

    // Add negative sign if needed
    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0'; // Null-terminate the string

    reverse(buffer, i); // Reverse the string

    return buffer;
}
char *itoa(int value, char *str, int base)
{
    switch(base) {
        case 10:
            return itoa_decimal(value, str);
        case 16:
            //return itoa_hex(value, str);
        case 2:
        case 8:
        default:
            str[0] = 'E';
            str[1] = '\0';
            return str;
    }
}
#ifdef __cplusplus
}
#endif
