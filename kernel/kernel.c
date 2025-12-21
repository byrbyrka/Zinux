#include "common.h"
#include "console.h"
#include "keyboard.h"
#include "string.h"

#define KERNEL_VERSION L"superpuperprealpharelease0.0.1"
#define KERNEL_NAME    L"Zinux Kernel"
#define CMD_BUFFER_SIZE 256

void cmd_help(void);
void cmd_clear(void);
void cmd_echo(const CHAR16 *args);
void cmd_info(void);
void cmd_reboot(void);
void cmd_shutdown(void);
void cmd_color(const CHAR16 *args);
void cmd_time(void);
void cmd_mem(void);

typedef struct {
    const CHAR16 *name;
    const CHAR16 *description;
    void (*handler)(const CHAR16 *args);
} Command;

void cmd_help_wrapper(const CHAR16 *args) { (void)args; cmd_help(); }
void cmd_clear_wrapper(const CHAR16 *args) { (void)args; cmd_clear(); }
void cmd_info_wrapper(const CHAR16 *args) { (void)args; cmd_info(); }
void cmd_reboot_wrapper(const CHAR16 *args) { (void)args; cmd_reboot(); }
void cmd_shutdown_wrapper(const CHAR16 *args) { (void)args; cmd_shutdown(); }
void cmd_time_wrapper(const CHAR16 *args) { (void)args; cmd_time(); }
void cmd_mem_wrapper(const CHAR16 *args) { (void)args; cmd_mem(); }

Command commands[] = {
    {L"help",     L"Show available commands",      cmd_help_wrapper},
    {L"clear",    L"Clear the screen",             cmd_clear_wrapper},
    {L"cls",      L"Clear the screen (alias)",     cmd_clear_wrapper},
    {L"echo",     L"Print text to screen",         cmd_echo},
    {L"info",     L"Show system information",      cmd_info_wrapper},
    {L"reboot",   L"Restart the computer",         cmd_reboot_wrapper},
    {L"shutdown", L"Shutdown the computer",        cmd_shutdown_wrapper},
    {L"color",    L"Change console colors",        cmd_color},
    {L"time",     L"Show current time",            cmd_time_wrapper},
    {L"mem",      L"Show memory information",      cmd_mem_wrapper},
    {NULL, NULL, NULL}
};

void print_welcome(void) {
    console_set_color(CONSOLE_LIGHTCYAN, CONSOLE_BLACK);
    console_println(L"");
    console_println(L"  ======================================================");
    console_println(L"  ||                                                  ||");
    console_print(L"    ||              Welcome to ");
    console_set_color(CONSOLE_YELLOW, CONSOLE_BLACK);
    console_print(KERNEL_NAME);
    console_set_color(CONSOLE_LIGHTCYAN, CONSOLE_BLACK);
    console_print(L" v");
    console_print(KERNEL_VERSION);
    console_println(L"                                                      ||");
    console_println(L"  ||                                                  ||");
    console_println(L"  ||          Samoe krutoe russkoe yadro ZZZ GOYDA    ||");
    console_println(L"  ||                                                  ||");
    console_println(L"  ======================================================");
    console_println(L"");
    console_reset_color();
    
    console_println(L"  Type 'help' to see available commands.");
    console_println(L"");
}

void parse_command(CHAR16 *cmdline) {
    CHAR16 cmd[64];
    CHAR16 *args = NULL;
    int i = 0;
    
    while (*cmdline == L' ') cmdline++;
    if (*cmdline == L'\0') return;
    
    while (*cmdline && *cmdline != L' ' && i < 63) {
        cmd[i++] = *cmdline++;
    }
    cmd[i] = L'\0';
    
    while (*cmdline == L' ') cmdline++;
    if (*cmdline) args = cmdline;
    
    for (int j = 0; commands[j].name != NULL; j++) {
        if (wstrcmp(cmd, commands[j].name) == 0) {
            commands[j].handler(args);
            return;
        }
    }
    
    console_set_color(CONSOLE_LIGHTRED, CONSOLE_BLACK);
    console_print(L"Unknown command: ");
    console_println(cmd);
    console_reset_color();
    console_println(L"Type 'help' for available commands.");
}

void cmd_help(void) {
    console_set_color(CONSOLE_LIGHTGREEN, CONSOLE_BLACK);
    console_println(L"");
    console_println(L"Available commands:");
    console_println(L"-------------------");
    console_reset_color();
    
    for (int i = 0; commands[i].name != NULL; i++) {
        console_set_color(CONSOLE_YELLOW, CONSOLE_BLACK);
        console_print(L"  ");
        console_print(commands[i].name);
        console_reset_color();
        
        UINTN len = wstrlen(commands[i].name);
        for (UINTN j = len; j < 12; j++) {
            console_putchar(L' ');
        }
        
        console_print(L"- ");
        console_println(commands[i].description);
    }
    console_println(L"");
}

void cmd_clear(void) {
    console_clear();
}

void cmd_echo(const CHAR16 *args) {
    if (args == NULL || *args == L'\0') {
        console_println(L"");
    } else {
        console_println(args);
    }
}

void cmd_info(void) {
    EFI_TIME time;
    
    console_set_color(CONSOLE_LIGHTGREEN, CONSOLE_BLACK);
    console_println(L"");
    console_println(L"System Information:");
    console_println(L"-------------------");
    console_reset_color();
    
    console_print(L"  OS Name:       ");
    console_set_color(CONSOLE_YELLOW, CONSOLE_BLACK);
    console_println(KERNEL_NAME);
    console_reset_color();
    
    console_print(L"  Version:       ");
    console_set_color(CONSOLE_YELLOW, CONSOLE_BLACK);
    console_println(KERNEL_VERSION);
    console_reset_color();
    
    console_print(L"  Architecture:  ");
    console_set_color(CONSOLE_YELLOW, CONSOLE_BLACK);
    console_println(L"x86_64 (UEFI)");
    console_reset_color();
    
    console_print(L"  UEFI Revision: ");
    console_set_color(CONSOLE_YELLOW, CONSOLE_BLACK);
    console_print_int(ST->Hdr.Revision >> 16);
    console_putchar(L'.');
    console_print_int(ST->Hdr.Revision & 0xFFFF);
    console_println(L"");
    console_reset_color();
    
    console_print(L"  Console Size:  ");
    console_set_color(CONSOLE_YELLOW, CONSOLE_BLACK);
    console_print_int(gConsole.cols);
    console_print(L" x ");
    console_print_int(gConsole.rows);
    console_println(L"");
    console_reset_color();
    
    if (uefi_call_wrapper(RT->GetTime, 2, &time, NULL) == EFI_SUCCESS) {
        console_print(L"  Date:          ");
        console_set_color(CONSOLE_YELLOW, CONSOLE_BLACK);
        console_print_int(time.Year);
        console_putchar(L'-');
        if (time.Month < 10) console_putchar(L'0');
        console_print_int(time.Month);
        console_putchar(L'-');
        if (time.Day < 10) console_putchar(L'0');
        console_print_int(time.Day);
        console_println(L"");
        console_reset_color();
    }
    
    console_println(L"");
}

void cmd_reboot(void) {
    console_println(L"");
    console_println(L"Rebooting system...");
    uefi_call_wrapper(RT->ResetSystem, 4, EfiResetCold, EFI_SUCCESS, 0, NULL);
}

void cmd_shutdown(void) {
    console_println(L"");
    console_println(L"Shutting down...");
    uefi_call_wrapper(RT->ResetSystem, 4, EfiResetShutdown, EFI_SUCCESS, 0, NULL);
}

void cmd_color(const CHAR16 *args) {
    if (args == NULL || *args == L'\0') {
        console_println(L"Usage: color <fg> [bg]");
        console_println(L"  Colors: 0-15 (0=black, 15=white)");
        console_println(L"  Example: color 14 1  (yellow on blue)");
        return;
    }
    
    UINTN fg = 0, bg = 0;
    
    while (*args >= L'0' && *args <= L'9') {
        fg = fg * 10 + (*args - L'0');
        args++;
    }
    
    while (*args == L' ') args++;
    
    if (*args) {
        while (*args >= L'0' && *args <= L'9') {
            bg = bg * 10 + (*args - L'0');
            args++;
        }
    }
    
    if (fg > 15) fg = 15;
    if (bg > 15) bg = 15;
    
    console_set_color(fg, bg);
    console_println(L"Color changed!");
}

void cmd_time(void) {
    EFI_TIME time;
    
    if (uefi_call_wrapper(RT->GetTime, 2, &time, NULL) != EFI_SUCCESS) {
        console_set_color(CONSOLE_LIGHTRED, CONSOLE_BLACK);
        console_println(L"Failed to get time");
        console_reset_color();
        return;
    }
    
    console_print(L"\nCurrent time: ");
    console_set_color(CONSOLE_YELLOW, CONSOLE_BLACK);
    
    if (time.Hour < 10) console_putchar(L'0');
    console_print_int(time.Hour);
    console_putchar(L':');
    if (time.Minute < 10) console_putchar(L'0');
    console_print_int(time.Minute);
    console_putchar(L':');
    if (time.Second < 10) console_putchar(L'0');
    console_print_int(time.Second);
    
    console_reset_color();
    console_println(L"");
}

void cmd_mem(void) {
    EFI_STATUS status;
    UINTN memMapSize = 0;
    EFI_MEMORY_DESCRIPTOR *memMap = NULL;
    UINTN mapKey;
    UINTN descSize;
    UINT32 descVersion;
    UINT64 totalMem = 0;
    UINT64 freeMem = 0;
    
    status = uefi_call_wrapper(BS->GetMemoryMap, 5, 
                               &memMapSize, memMap, &mapKey, &descSize, &descVersion);
    
    if (status == EFI_BUFFER_TOO_SMALL) {
        memMapSize += 2 * descSize;
        status = uefi_call_wrapper(BS->AllocatePool, 3, 
                                   EfiLoaderData, memMapSize, (void**)&memMap);
        
        if (EFI_ERROR(status)) {
            console_println(L"Failed to allocate memory for memory map");
            return;
        }
        
        status = uefi_call_wrapper(BS->GetMemoryMap, 5,
                                   &memMapSize, memMap, &mapKey, &descSize, &descVersion);
    }
    
    if (EFI_ERROR(status)) {
        console_println(L"Failed to get memory map");
        return;
    }
    
    UINTN numEntries = memMapSize / descSize;
    EFI_MEMORY_DESCRIPTOR *desc = memMap;
    
    for (UINTN i = 0; i < numEntries; i++) {
        UINT64 size = desc->NumberOfPages * 4096;
        totalMem += size;
        
        if (desc->Type == EfiConventionalMemory ||
            desc->Type == EfiBootServicesCode ||
            desc->Type == EfiBootServicesData) {
            freeMem += size;
        }
        
        desc = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)desc + descSize);
    }
    
    console_set_color(CONSOLE_LIGHTGREEN, CONSOLE_BLACK);
    console_println(L"");
    console_println(L"Memory Information:");
    console_println(L"-------------------");
    console_reset_color();
    
    console_print(L"  Total Memory:     ");
    console_set_color(CONSOLE_YELLOW, CONSOLE_BLACK);
    console_print_int(totalMem / (1024 * 1024));
    console_println(L" MB");
    console_reset_color();
    
    console_print(L"  Available Memory: ");
    console_set_color(CONSOLE_YELLOW, CONSOLE_BLACK);
    console_print_int(freeMem / (1024 * 1024));
    console_println(L" MB");
    console_reset_color();
    
    console_print(L"  Memory Regions:   ");
    console_set_color(CONSOLE_YELLOW, CONSOLE_BLACK);
    console_print_int(numEntries);
    console_println(L"");
    console_reset_color();
    
    console_println(L"");
    
    uefi_call_wrapper(BS->FreePool, 1, memMap);
}

void shell_run(void) {
    CHAR16 cmdline[CMD_BUFFER_SIZE];
    
    while (1) {
        console_set_color(CONSOLE_LIGHTGREEN, CONSOLE_BLACK);
        console_print(KERNEL_NAME);
        console_set_color(CONSOLE_LIGHTGRAY, CONSOLE_BLACK);
        console_print(L"> ");
        console_reset_color();
        
        keyboard_read_line(cmdline, CMD_BUFFER_SIZE);
        parse_command(cmdline);
    }
}

void kernel_main(void) {
    console_init();
    keyboard_init();
    print_welcome();
    shell_run();
}