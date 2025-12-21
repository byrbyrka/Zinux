#include "console.h"
#include "string.h"

Console gConsole;

EFI_STATUS console_init(void) {
    EFI_STATUS status;
    
    gConsole.output = ST->ConOut;
    gConsole.input = ST->ConIn;
    
    status = uefi_call_wrapper(gConsole.output->QueryMode, 4,
        gConsole.output,
        gConsole.output->Mode->Mode,
        &gConsole.cols,
        &gConsole.rows
    );
    
    if (EFI_ERROR(status)) {
        gConsole.cols = 80;
        gConsole.rows = 25;
    }
    
    gConsole.cursor_x = 0;
    gConsole.cursor_y = 0;
    gConsole.attribute = CONSOLE_ATTR(CONSOLE_LIGHTGRAY, CONSOLE_BLACK);
    
    // Очистка и сброс
    uefi_call_wrapper(gConsole.output->ClearScreen, 1, gConsole.output);
    uefi_call_wrapper(gConsole.output->EnableCursor, 2, gConsole.output, TRUE);
    uefi_call_wrapper(gConsole.output->SetAttribute, 2, gConsole.output, gConsole.attribute);
    
    return EFI_SUCCESS;
}

void console_clear(void) {
    uefi_call_wrapper(gConsole.output->ClearScreen, 1, gConsole.output);
    gConsole.cursor_x = 0;
    gConsole.cursor_y = 0;
}

void console_set_color(UINTN foreground, UINTN background) {
    gConsole.attribute = CONSOLE_ATTR(foreground, background);
    uefi_call_wrapper(gConsole.output->SetAttribute, 2, gConsole.output, gConsole.attribute);
}

void console_reset_color(void) {
    console_set_color(CONSOLE_LIGHTGRAY, CONSOLE_BLACK);
}

void console_set_cursor(UINTN x, UINTN y) {
    gConsole.cursor_x = x;
    gConsole.cursor_y = y;
    uefi_call_wrapper(gConsole.output->SetCursorPosition, 3, gConsole.output, x, y);
}

void console_get_cursor(UINTN *x, UINTN *y) {
    *x = gConsole.cursor_x;
    *y = gConsole.cursor_y;
}

void console_scroll(void) {
    if (gConsole.cursor_y >= gConsole.rows) {
        gConsole.cursor_y = gConsole.rows - 1;
    }
}

void console_newline(void) {
    CHAR16 newline[] = {L'\r', L'\n', L'\0'};
    uefi_call_wrapper(gConsole.output->OutputString, 2, gConsole.output, newline);
    gConsole.cursor_x = 0;
    gConsole.cursor_y++;
    if (gConsole.cursor_y >= gConsole.rows) {
        gConsole.cursor_y = gConsole.rows - 1;
    }
}

void console_backspace(void) {
    if (gConsole.cursor_x > 0) {
        gConsole.cursor_x--;
        uefi_call_wrapper(gConsole.output->SetCursorPosition, 3, 
                          gConsole.output, gConsole.cursor_x, gConsole.cursor_y);
        CHAR16 space[] = {L' ', L'\0'};
        uefi_call_wrapper(gConsole.output->OutputString, 2, gConsole.output, space);
        uefi_call_wrapper(gConsole.output->SetCursorPosition, 3,
                          gConsole.output, gConsole.cursor_x, gConsole.cursor_y);
    }
}

void console_putchar(CHAR16 c) {
    CHAR16 str[2] = {c, L'\0'};
    
    if (c == L'\n') {
        console_newline();
        return;
    }
    
    if (c == L'\r') {
        gConsole.cursor_x = 0;
        return;
    }
    
    if (c == L'\t') {
        for (int i = 0; i < 4; i++) {
            console_putchar(L' ');
        }
        return;
    }
    
    uefi_call_wrapper(gConsole.output->OutputString, 2, gConsole.output, str);
    gConsole.cursor_x++;
    
    if (gConsole.cursor_x >= gConsole.cols) {
        gConsole.cursor_x = 0;
        gConsole.cursor_y++;
        if (gConsole.cursor_y >= gConsole.rows) {
            gConsole.cursor_y = gConsole.rows - 1;
        }
    }
}

void console_print(const CHAR16 *str) {
    if (str == NULL) return;
    uefi_call_wrapper(gConsole.output->OutputString, 2, gConsole.output, (CHAR16*)str);
    // Обновляем позицию курсора
    while (*str) {
        if (*str == L'\n') {
            gConsole.cursor_x = 0;
            gConsole.cursor_y++;
        } else {
            gConsole.cursor_x++;
            if (gConsole.cursor_x >= gConsole.cols) {
                gConsole.cursor_x = 0;
                gConsole.cursor_y++;
            }
        }
        str++;
    }
}

void console_println(const CHAR16 *str) {
    if (str != NULL) {
        console_print(str);
    }
    console_newline();
}

void console_print_int(INT64 value) {
    CHAR16 buffer[32];
    itow(value, buffer, 10);
    console_print(buffer);
}

void console_print_hex(UINT64 value) {
    CHAR16 buffer[32];
    console_print(L"0x");
    itow((INT64)value, buffer, 16);
    console_print(buffer);
}

void console_printf(const CHAR16 *format, ...) {
    if (format != NULL) {
        console_print(format);
    }
}