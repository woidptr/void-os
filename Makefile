PROFILE ?= debug

CC = x86_64-elf-gcc
AS = nasm

CFLAGS = -std=gnu23 -ffreestanding -Wall -Wextra \
         -m64 -march=x86-64 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
         -mcmodel=kernel -fno-pic -fno-pie -Ivendors/limine -Isrc \
		 --embed-dir=assets
ASFLAGS = -f elf64
LDFLAGS = -T linker.ld -nostdlib -z max-page-size=0x1000

ifeq ($(PROFILE),debug)
	CFLAGS += -O0 -g
else ifeq ($(PROFILE),release)
	CFLAGS += -O2 -DNDEBUG
else
	$(error "Invalid PROFILE!")
endif

REQUIRED_BINS := $(CC) $(AS) xorriso qemu-system-x86_64 uv
$(foreach bin,$(REQUIRED_BINS),\
    $(if $(shell command -v $(bin) 2> /dev/null),,\
        $(error "Missing dependency: [$(bin)] is not installed or not in PATH.")))

BUILD_DIR = build/$(PROFILE)
SRC_DIR = src
ISO_DIR = iso_root
TARGET_ELF = $(BUILD_DIR)/kernel.elf
TARGET_ISO = $(BUILD_DIR)/myos.iso
LIMINE_DIR = vendors/limine

C_SRCS := $(shell find $(SRC_DIR) -name '*.c')
C_OBJS := $(C_SRCS:%.c=$(BUILD_DIR)/%.o)

AS_SRCS := $(shell find $(SRC_DIR) -name '*.asm')
AS_OBJS := $(AS_SRCS:%.asm=$(BUILD_DIR)/%.o)

OBJECTS := $(C_OBJS) $(AS_OBJS)

.PHONY: all run clean

all: $(TARGET_ISO)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(TARGET_ELF): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET_ISO): $(TARGET_ELF) limine.conf
	@uv run --project builder -m builder --rootdir $(CURDIR)

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

build: $(TARGET_ISO)
	@echo "Build files have successfully been written to $(TARGET_ISO)"

run: $(TARGET_ISO)
	qemu-system-x86_64 -cdrom $(TARGET_ISO) -m 256M -debugcon stdio

clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR) $(TARGET_ISO)
