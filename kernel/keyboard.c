#include "keyboard.h"
#include "console.h"
#include "string.h"

EFI_STATUS keyboard_init(void) {
    keyboard_flush();
    return EFI_SUCCESS;
}

void keyboard_flush(void) {
    EFI_INPUT_KEY key;
    while (uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2, ST->ConIn, &key) == EFI_SUCCESS) {
    }
}

BOOLEAN keyboard_has_key(void) {
    EFI_STATUS status;
    status = uefi_call_wrapper(BS->CheckEvent, 1, ST->ConIn->WaitForKey);
    return (status == EFI_SUCCESS);
}

KeyEvent keyboard_wait_key(void) {
    EFI_STATUS status;
    EFI_INPUT_KEY efi_key;
    KeyEvent event;
    UINTN index;
    
    uefi_call_wrapper(BS->WaitForEvent, 3, 1, &ST->ConIn->WaitForKey, &index);
    status = uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2, ST->ConIn, &efi_key);
    
    if (EFI_ERROR(status)) {
        event.unicode = 0;
        event.scan_code = 0;
        event.is_special = FALSE;
        return event;
    }
    
    event.unicode = efi_key.UnicodeChar;
    event.scan_code = efi_key.ScanCode;
    
    if (efi_key.UnicodeChar == 0) {
        event.is_special = TRUE;
        switch (efi_key.ScanCode) {
            case SCAN_UP:        event.scan_code = KEY_UP; break;
            case SCAN_DOWN:      event.scan_code = KEY_DOWN; break;
            case SCAN_LEFT:      event.scan_code = KEY_LEFT; break;
            case SCAN_RIGHT:     event.scan_code = KEY_RIGHT; break;
            case SCAN_HOME:      event.scan_code = KEY_HOME; break;
            case SCAN_END:       event.scan_code = KEY_END; break;
            case SCAN_INSERT:    event.scan_code = KEY_INSERT; break;
            case SCAN_DELETE:    event.scan_code = KEY_DELETE; break;
            case SCAN_PAGE_UP:   event.scan_code = KEY_PGUP; break;
            case SCAN_PAGE_DOWN: event.scan_code = KEY_PGDN; break;
            case SCAN_F1:        event.scan_code = KEY_F1; break;
            case SCAN_F2:        event.scan_code = KEY_F2; break;
            case SCAN_F3:        event.scan_code = KEY_F3; break;
            case SCAN_F4:        event.scan_code = KEY_F4; break;
            case SCAN_F5:        event.scan_code = KEY_F5; break;
            case SCAN_F6:        event.scan_code = KEY_F6; break;
            case SCAN_F7:        event.scan_code = KEY_F7; break;
            case SCAN_F8:        event.scan_code = KEY_F8; break;
            case SCAN_F9:        event.scan_code = KEY_F9; break;
            case SCAN_F10:       event.scan_code = KEY_F10; break;
            case SCAN_F11:       event.scan_code = KEY_F11; break;
            case SCAN_F12:       event.scan_code = KEY_F12; break;
            case SCAN_ESC:       event.scan_code = KEY_ESC; break;
            default:             event.scan_code = efi_key.ScanCode; break;
        }
    } else {
        event.is_special = FALSE;
    }
    
    return event;
}

BOOLEAN keyboard_get_key(KeyEvent *event) {
    EFI_STATUS status;
    EFI_INPUT_KEY efi_key;
    
    status = uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2, ST->ConIn, &efi_key);
    
    if (EFI_ERROR(status)) {
        return FALSE;
    }
    
    event->unicode = efi_key.UnicodeChar;
    event->scan_code = efi_key.ScanCode;
    event->is_special = (efi_key.UnicodeChar == 0);
    
    return TRUE;
}

void keyboard_read_line(CHAR16 *buffer, UINTN max_len) {
    UINTN pos = 0;
    KeyEvent key;
    
    // Правильное обнуление CHAR16 буфера
    wmemset(buffer, L'\0', max_len);
    
    while (1) {
        key = keyboard_wait_key();
        
        if (key.is_special) {
            switch (key.scan_code) {
                case KEY_ESC:
                    while (pos > 0) {
                        pos--;
                        console_backspace();
                    }
                    buffer[0] = L'\0';
                    break;
                default:
                    break;
            }
            continue;
        }
        
        // Enter
        if (key.unicode == L'\r' || key.unicode == L'\n') {
            buffer[pos] = L'\0';
            console_newline();
            return;
        }
        
        // Backspace
        if (key.unicode == 0x08) {
            if (pos > 0) {
                pos--;
                buffer[pos] = L'\0';
                console_backspace();
            }
            continue;
        }
        
        // Tab
        if (key.unicode == 0x09) {
            UINTN spaces = 4;
            while (spaces-- > 0 && pos < max_len - 1) {
                buffer[pos++] = L' ';
                console_putchar(L' ');
            }
            continue;
        }
        
        // Обычные символы
        if (pos < max_len - 1 && key.unicode >= 0x20) {
            buffer[pos++] = key.unicode;
            console_putchar(key.unicode);
        }
    }
}