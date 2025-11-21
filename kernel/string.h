#pragma once

#ifdef __cplusplus
extern "C" {
#endif
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

#ifdef __cplusplus
}
#endif
