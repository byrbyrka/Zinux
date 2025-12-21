# Zinux - Analog "Linux" #1 in Russia

## ![Logo Image](assets/logo1-256.png)

## --------- Check List ---------

### Working bootloader - 🟥 (No image)

### Working kernel - 🟥

### Working drivers for keyboard - 🟥

### Your own extension for launching applications (analog .exe, .appimage) - 🟥

### Attempts to boot on real hardware - 🟥 (There were no attempts)

## --------- Build & Run ---------

### Prerequisites

- NASM (>= 2.15)
- QEMU (>= 5.0)
- GNU Make

```bash
sudo pacman -S gnu-efi make nasm gcc qemu-full edk2-ovmf dosfstools
```

### build
```bash
make clean && make
chmod +x create_disk.sh
sudo ./create_disk.sh
```

### Start in QEMU
Method 1:
```bash
make run
```

Method 2:
```bash
qemu-system-x86_64 -bios /usr/share/edk2-ovmf/x64/OVMF.4m.fd -drive file=disk.img,format=raw,if=ide -m 256M -serial stdio
```

### Clean
To remove all build artifacts run:
```bash
make clean
```

---

If everything works, you should see the bootloader starting Zinux.

## --------- Social ---------

- [Telegram](https://t.me/Zinux_channel)

- [Github](https://github.com/Norton42qq/Zinux/issues)

## --------- Star History ---------

<a href="https://www.star-history.com/#norton42qq/zinux&type=date&legend=top-left">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/svg?repos=norton42qq/zinux&type=date&theme=dark&legend=top-left" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/svg?repos=norton42qq/zinux&type=date&legend=top-left" />
   <img alt="Star History Chart" src="https://api.star-history.com/svg?repos=norton42qq/zinux&type=date&legend=top-left" />
 </picture>
</a>
