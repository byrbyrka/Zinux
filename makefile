ARCH = x86_64
GNUEFI = /usr/include/efi
GNUEFI_LIB = /usr/lib

CC = gcc
LD = ld
OBJCOPY = objcopy

CFLAGS = -I$(GNUEFI) \
         -I$(GNUEFI)/$(ARCH) \
         -I./include \
         -I./kernel \
         -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -Wall \
         -Wextra -ffreestanding -fno-builtin -maccumulate-outgoing-args -c

LDFLAGS = -nostdlib -znocombreloc -T $(GNUEFI_LIB)/elf_$(ARCH)_efi.lds \
          -shared -Bsymbolic -L $(GNUEFI_LIB) $(GNUEFI_LIB)/crt0-efi-$(ARCH).o

LIBS = -lefi -lgnuefi

BOOT_EFI = build/BOOTX64.EFI
BOOT_SO = build/boot.so
OVMF = /usr/share/edk2-ovmf/x64/OVMF.4m.fd


OBJS = build/boot.o build/kernel.o build/console.o build/keyboard.o build/string.o

.PHONY: all clean run dirs

all: dirs $(BOOT_EFI) disk.img

dirs:
	@mkdir -p build



build/boot.o: bootloader/boot.c
	$(CC) $(CFLAGS) $< -o $@

build/kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) $< -o $@

build/console.o: kernel/console.c
	$(CC) $(CFLAGS) $< -o $@

build/keyboard.o: kernel/keyboard.c
	$(CC) $(CFLAGS) $< -o $@

build/string.o: kernel/string.c
	$(CC) $(CFLAGS) $< -o $@



$(BOOT_SO): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)



$(BOOT_EFI): $(BOOT_SO)
	$(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic -j .dynsym \
	           -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc \
	           --target efi-app-$(ARCH) $< $@
	@echo "Created: $@"
	@file $@



disk.img: $(BOOT_EFI)
	@echo "Creating disk image..."
	@dd if=/dev/zero of=disk.img bs=1M count=64 status=none
	@echo "Creating FAT32 filesystem..."
	@mkfs.vfat -F 32 -n UEFI disk.img > /dev/null 2>&1
	@echo "Copying EFI file..."
	@mmd -i disk.img ::EFI || true
	@mmd -i disk.img ::EFI/BOOT || true
	@mcopy -i disk.img $(BOOT_EFI) ::EFI/BOOT/BOOTX64.EFI
	@echo "Verifying..."
	@mdir -i disk.img ::EFI/BOOT/
	@echo "Disk image ready!"



run: disk.img
	@echo "Starting QEMU with OVMF..."
	qemu-system-x86_64 \
		-bios $(OVMF) \
		-drive file=disk.img,format=raw,if=ide \
		-net none \
		-m 256M \
		-serial stdio



clean:
	rm -rf build/* disk.img esp
