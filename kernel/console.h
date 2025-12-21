#ifndef CONSOLE_H
#define CONSOLE_H

#include "common.h"

// Цвета консоли
#define CONSOLE_BLACK         0x00
#define CONSOLE_BLUE          0x01
#define CONSOLE_GREEN         0x02
#define CONSOLE_CYAN          0x03
#define CONSOLE_RED           0x04
#define CONSOLE_MAGENTA       0x05
#define CONSOLE_BROWN         0x06
#define CONSOLE_LIGHTGRAY     0x07
#define CONSOLE_DARKGRAY      0x08
#define CONSOLE_LIGHTBLUE     0x09
#define CONSOLE_LIGHTGREEN    0x0A
#define CONSOLE_LIGHTCYAN     0x0B
#define CONSOLE_LIGHTRED      0x0C
#define CONSOLE_LIGHTMAGENTA  0x0D
#define CONSOLE_YELLOW        0x0E
#define CONSOLE_WHITE         0x0F

// Макрос для создания атрибута цвета
#define CONSOLE_ATTR(fg, bg) ((bg << 4) | fg)

// Структура консоли
typedef struct {
    SIMPLE_TEXT_OUTPUT_INTERFACE *output;
    SIMPLE_INPUT_INTERFACE *input;
    UINTN cols;
    UINTN rows;
    UINTN cursor_x;
    UINTN cursor_y;
    UINTN attribute;
} Console;

// Глобальная консоль
extern Console gConsole;

// Инициализация консоли
EFI_STATUS console_init(void);

// Очистка экрана
void console_clear(void);

// Установка цвета
void console_set_color(UINTN foreground, UINTN background);

// Сброс цвета на стандартный
void console_reset_color(void);

// Установка позиции курсора
void console_set_cursor(UINTN x, UINTN y);

// Получение позиции курсора
void console_get_cursor(UINTN *x, UINTN *y);

// Вывод символа
void console_putchar(CHAR16 c);

// Вывод строки
void console_print(const CHAR16 *str);

// Вывод строки с переводом строки
void console_println(const CHAR16 *str);

// Вывод числа
void console_print_int(INT64 value);

// Вывод числа в hex
void console_print_hex(UINT64 value);

// Форматированный вывод (упрощённый)
void console_printf(const CHAR16 *format, ...);

// Прокрутка экрана
void console_scroll(void);

// Новая строка
void console_newline(void);

// Backspace
void console_backspace(void);

#endif // CONSOLE_H