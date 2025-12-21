#ifndef COMMON_H
#define COMMON_H

#include <efi.h>
#include <efilib.h>

extern EFI_HANDLE gImageHandle;

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#endif