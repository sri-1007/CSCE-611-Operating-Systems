sudo mount -o loop dev_kernel_grub.img /mnt/floppy
sudo cp kernel.elf /mnt/floppy/
sleep 1s
sudo umount /mnt/floppy/
