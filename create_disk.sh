#!/bin/bash

set -e

EFI_FILE="build/BOOTX64.EFI"
DISK_IMG="disk.img"

if [ ! -f "$EFI_FILE" ]; then
    echo "Error: $EFI_FILE not found! Run 'make' first."
    exit 1
fi

rm -f $DISK_IMG

dd if=/dev/zero of=$DISK_IMG bs=1M count=64 status=progress

parted -s $DISK_IMG mklabel gpt
parted -s $DISK_IMG mkpart primary fat32 1MiB 63MiB
parted -s $DISK_IMG set 1 boot on

LOOP=$(sudo losetup --find --show --partscan $DISK_IMG)
echo "    Loop device: $LOOP"

sleep 1

PART="${LOOP}p1"
if [ ! -b "$PART" ]; then
    echo "    Partition not found, trying alternative..."
    sudo partprobe $LOOP
    sleep 1
fi

if [ ! -b "$PART" ]; then
    echo "Error: Partition $PART not found!"
    sudo losetup -d $LOOP
    exit 1
fi

sudo mkfs.vfat -F 32 -n UEFI $PART

MOUNT_POINT=$(mktemp -d)
sudo mount $PART $MOUNT_POINT

sudo mkdir -p $MOUNT_POINT/EFI/BOOT
sudo cp $EFI_FILE $MOUNT_POINT/EFI/BOOT/BOOTX64.EFI

sudo ls -laR $MOUNT_POINT/

sudo umount $MOUNT_POINT
rmdir $MOUNT_POINT
sudo losetup -d $LOOP
sudo chmod 777 disk.img

echo "Done"
