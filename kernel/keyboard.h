#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "common.h"

#define KEY_NULL        0x0000
#define KEY_UP          0x0001
#define KEY_DOWN        0x0002
#define KEY_LEFT        0x0003
#define KEY_RIGHT       0x0004
#define KEY_HOME        0x0005
#define KEY_END         0x0006
#define KEY_INSERT      0x0007
#define KEY_DELETE      0x0008
#define KEY_PGUP        0x0009
#define KEY_PGDN        0x000A
#define KEY_F1          0x000B
#define KEY_F2          0x000C
#define KEY_F3          0x000D
#define KEY_F4          0x000E
#define KEY_F5          0x000F
#define KEY_F6          0x0010
#define KEY_F7          0x0011
#define KEY_F8          0x0012
#define KEY_F9          0x0013
#define KEY_F10         0x0014
#define KEY_F11         0x0015
#define KEY_F12         0x0016
#define KEY_ESC         0x0017
#define KEY_BACKSPACE   0x0008
#define KEY_TAB         0x0009
#define KEY_ENTER       0x000D

// Структура для хранения клавиш
typedef struct {
    CHAR16 unicode;       // Юникод символ
    UINT16 scan_code;     // Scan code для специальных клавиш
    BOOLEAN is_special;   // Флаг специальной клавиши
} KeyEvent;

// Инициализация клавиатуры
EFI_STATUS keyboard_init(void);

// Проверка, есть ли нажатая клавиша
BOOLEAN keyboard_has_key(void);

// Ожидание нажатия клавиши (блокирующее)
KeyEvent keyboard_wait_key(void);

// Получение клавиши без ожидания
BOOLEAN keyboard_get_key(KeyEvent *event);

// Чтение строки с клавиатуры
void keyboard_read_line(CHAR16 *buffer, UINTN max_len);

// Сброс буфера клавиатуры
void keyboard_flush(void);

#endif