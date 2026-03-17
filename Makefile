CC = x86_64-elf-gcc
# Added -Ivendors/limine so it can find limine.h!
CFLAGS = -std=gnu23 -ffreestanding -O2 -Wall -Wextra \
         -m64 -march=x86-64 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
         -mcmodel=kernel -fno-pic -fno-pie -Ivendors/limine
LDFLAGS = -T linker.ld -nostdlib -z max-page-size=0x1000

BUILD_DIR = build
SRC_DIR = src
ISO_DIR = iso_root
TARGET_ELF = $(BUILD_DIR)/kernel.elf
TARGET_ISO = myos.iso
LIMINE_DIR = vendors/limine

SOURCES := $(shell find $(SRC_DIR) -name '*.c')

OBJECTS := $(SOURCES:%.c=$(BUILD_DIR)/%.o)

all: $(TARGET_ISO)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_ELF): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

# The rest remains exactly the same
$(TARGET_ISO): $(TARGET_ELF) limine.conf
	rm -rf $(ISO_DIR)
	mkdir -p $(ISO_DIR)/boot/limine
	mkdir -p $(ISO_DIR)/EFI/BOOT
	cp $(TARGET_ELF) $(ISO_DIR)/boot/
	cp limine.conf $(ISO_DIR)/boot/limine/
	
	cp $(LIMINE_DIR)/limine-bios.sys $(ISO_DIR)/boot/limine/
	cp $(LIMINE_DIR)/limine-bios-cd.bin $(ISO_DIR)/boot/limine/
	cp $(LIMINE_DIR)/BOOTX64.EFI $(ISO_DIR)/EFI/BOOT/
	cp $(LIMINE_DIR)/BOOTIA32.EFI $(ISO_DIR)/EFI/BOOT/
	
	xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot EFI/BOOT/BOOTX64.EFI \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(ISO_DIR) -o $(TARGET_ISO)

run: $(TARGET_ISO)
	qemu-system-x86_64 -cdrom $(TARGET_ISO) -m 256M

clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR) $(TARGET_ISO)

.PHONY: all run clean
