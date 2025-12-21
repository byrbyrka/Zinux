#ifndef STRING_H
#define STRING_H

#include "common.h"

UINTN strlen_ascii(const char *str);
UINTN wstrlen(const CHAR16 *str);

char* strcpy_ascii(char *dest, const char *src);
CHAR16* wstrcpy(CHAR16 *dest, const CHAR16 *src);

int strcmp_ascii(const char *s1, const char *s2);
int wstrcmp(const CHAR16 *s1, const CHAR16 *s2);

char* strcat_ascii(char *dest, const char *src);
CHAR16* wstrcat(CHAR16 *dest, const CHAR16 *src);

void* memset_custom(void *ptr, int value, UINTN num);
void* memcpy_custom(void *dest, const void *src, UINTN num);

// Для CHAR16 буферов
void wmemset(CHAR16 *ptr, CHAR16 value, UINTN count);

void itoa_ascii(INT64 value, char *str, int base);
void itow(INT64 value, CHAR16 *str, int base);

void char_to_wchar(const char *src, CHAR16 *dest, UINTN max_len);

#endif