#include <efi.h>
#include <efilib.h>

EFI_HANDLE gImageHandle;

extern void kernel_main(void);

EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    EFI_STATUS status;
    
    InitializeLib(ImageHandle, SystemTable);
    gImageHandle = ImageHandle;
    uefi_call_wrapper(BS->SetWatchdogTimer, 4, 0, 0, 0, NULL);
    
    // установка текстового режима
    {
        UINTN maxMode = ST->ConOut->Mode->MaxMode;
        UINTN bestMode = 0;
        UINTN bestCols = 0;
        UINTN bestRows = 0;
        
        for (UINTN mode = 0; mode < maxMode; mode++) {
            UINTN cols, rows;
            status = uefi_call_wrapper(ST->ConOut->QueryMode, 4, 
                                       ST->ConOut, mode, &cols, &rows);
            if (!EFI_ERROR(status)) {
                if (cols * rows > bestCols * bestRows) {
                    bestMode = mode;
                    bestCols = cols;
                    bestRows = rows;
                }
            }
        }
        
        if (bestCols > 0) {
            uefi_call_wrapper(ST->ConOut->SetMode, 2, ST->ConOut, bestMode);
        }
    }
    
    // клеар
    uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);
    
    // вывод загрузки
    uefi_call_wrapper(ST->ConOut->SetAttribute, 2, ST->ConOut, 
                      EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK);
    Print(L"UEFI Bootloader v1.0\r\n");
    Print(L"Loading kernel...\r\n\r\n");
    uefi_call_wrapper(ST->ConOut->SetAttribute, 2, ST->ConOut,
                      EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
    
    // небольшая задержка
    uefi_call_wrapper(BS->Stall, 1, 500000);
    
    // передача управления ядру
    kernel_main();
    
    // если ядро вернуло управление и не загрузилось
    Print(L"\r\n\r\nKernel returned unexpectedly!\r\n");
    Print(L"Press any key to reboot...\r\n");
    
    UINTN index;
    uefi_call_wrapper(BS->WaitForEvent, 3, 1, &ST->ConIn->WaitForKey, &index);    
    uefi_call_wrapper(RT->ResetSystem, 4, EfiResetCold, EFI_SUCCESS, 0, NULL);

    return EFI_SUCCESS;
}