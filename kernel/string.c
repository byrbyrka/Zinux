#include "string.h"

UINTN strlen_ascii(const char *str) {
    UINTN len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

UINTN wstrlen(const CHAR16 *str) {
    UINTN len = 0;
    while (str[len] != L'\0') {
        len++;
    }
    return len;
}

char* strcpy_ascii(char *dest, const char *src) {
    char *ret = dest;
    while ((*dest++ = *src++) != '\0');
    return ret;
}

CHAR16* wstrcpy(CHAR16 *dest, const CHAR16 *src) {
    CHAR16 *ret = dest;
    while ((*dest++ = *src++) != L'\0');
    return ret;
}

int strcmp_ascii(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

int wstrcmp(const CHAR16 *s1, const CHAR16 *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

char* strcat_ascii(char *dest, const char *src) {
    char *ret = dest;
    while (*dest) dest++;
    while ((*dest++ = *src++) != '\0');
    return ret;
}

CHAR16* wstrcat(CHAR16 *dest, const CHAR16 *src) {
    CHAR16 *ret = dest;
    while (*dest) dest++;
    while ((*dest++ = *src++) != L'\0');
    return ret;
}

void* memset_custom(void *ptr, int value, UINTN num) {
    UINT8 *p = (UINT8*)ptr;
    UINT8 val = (UINT8)value;
    for (UINTN i = 0; i < num; i++) {
        p[i] = val;
    }
    return ptr;
}

void* memcpy_custom(void *dest, const void *src, UINTN num) {
    UINT8 *d = (UINT8*)dest;
    const UINT8 *s = (const UINT8*)src;
    for (UINTN i = 0; i < num; i++) {
        d[i] = s[i];
    }
    return dest;
}

void wmemset(CHAR16 *ptr, CHAR16 value, UINTN count) {
    for (UINTN i = 0; i < count; i++) {
        ptr[i] = value;
    }
}

void itoa_ascii(INT64 value, char *str, int base) {
    char *ptr = str;
    char *ptr1 = str;
    char tmp_char;
    INT64 tmp_value;
    
    if (value < 0 && base == 10) {
        *ptr++ = '-';
        ptr1++;
        value = -value;
    }
    
    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return;
    }
    
    while (value) {
        tmp_value = value % base;
        if (tmp_value < 10) {
            *ptr++ = '0' + tmp_value;
        } else {
            *ptr++ = 'A' + tmp_value - 10;
        }
        value /= base;
    }
    
    *ptr-- = '\0';
    
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

void itow(INT64 value, CHAR16 *str, int base) {
    CHAR16 *ptr = str;
    CHAR16 *ptr1 = str;
    CHAR16 tmp_char;
    INT64 tmp_value;
    
    if (value < 0 && base == 10) {
        *ptr++ = L'-';
        ptr1++;
        value = -value;
    }
    
    if (value == 0) {
        *ptr++ = L'0';
        *ptr = L'\0';
        return;
    }
    
    while (value) {
        tmp_value = value % base;
        if (tmp_value < 10) {
            *ptr++ = L'0' + tmp_value;
        } else {
            *ptr++ = L'A' + tmp_value - 10;
        }
        value /= base;
    }
    
    *ptr-- = L'\0';
    
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

void char_to_wchar(const char *src, CHAR16 *dest, UINTN max_len) {
    UINTN i = 0;
    while (src[i] != '\0' && i < max_len - 1) {
        dest[i] = (CHAR16)src[i];
        i++;
    }
    dest[i] = L'\0';
}