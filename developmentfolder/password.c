#include "zxe_api.h"

// В Zinux SDK нет встроенного генератора псевдослучайных чисел, пишем свой LCG
static uint32_t prng_seed = 1;

void seed_rand(uint32_t seed) {
    prng_seed = seed;
}

uint32_t get_rand() {
    // Классические константы для линейного конгруэнтного метода
    prng_seed = prng_seed * 1664525 + 1013904223;
    return prng_seed;
}

int main(int argc, char* argv[]) {
    // Очистка экрана и подготовка цвета
    clear();
    color(WHITE, BLACK);

    // Экранируем бэкслеши для C-строки
    const char* logo = 
        "            ___\n"
        "           / | \\\n"
        "          |     |\n"
        "          |_____|\n"
        "          |     |\n"
        "          |     |\n"
        "          |     |\n"
        "          |     |\n"
        "          |     |\n"
        "          |     |\n"
        "      ___ |     | ___\n"
        "     /   \\       /   \\\n"
        "    |     |     |     |\n"
        "     \\___/       \\___/\n";

    println(logo);

    // Сидируем рандом текущим временем из RTC, чтобы пароли не повторялись
    uint8_t h, m, s, d, mo;
    uint16_t y;
    get_time(&h, &m, &s);
    get_date(&d, &mo, &y);
    seed_rand((uint32_t)h * 3600 + (uint32_t)m * 60 + s + d + mo + y);

    println("Введите длинну пароля:");
    
    char dlinna_str[16];
    input(dlinna_str, 15);

    int dlinna = to_int(dlinna_str);

    // Эмуляция `expect()` из Rust с оригинальными текстами
    if (slen(dlinna_str) == 0) {
        println("Нормально напиши еблан");
        waitkey();
        return 0; // Корректный выход через start.asm
    }
    
    if (dlinna <= 0) {
        println("Число ввести надо а не эту залупу");
        waitkey();
        return 0;
    }

    // В Zinux нет malloc, выделяем память на стеке
    if (dlinna > 255) {
        dlinna = 255;
    }

    const char* charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int charset_len = slen(charset);
    
    char password[256];
    
    // Аналог Alphanumeric.sample_string
    for (int i = 0; i < dlinna; i++) {
        uint32_t rand_val = get_rand();
        password[i] = charset[rand_val % charset_len];
    }
    password[dlinna] = '\0'; // Обязательный null-терминатор для C-строки

    print("Ваш пароль: ");
    println(password);

    // Пауза, чтобы программа сразу не закрылась и не выбросила обратно в шелл
    println("");
    println("Нажмите любую клавишу для выхода...");
    waitkey();

    return 0;
}