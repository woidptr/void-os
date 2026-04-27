PROFILE ?= debug
TARGET_ARCH ?= x86

CC := $(shell brew --prefix llvm)/bin/clang
CXX := $(shell brew --prefix llvm)/bin/clang++
AS := $(CC)

COMMON_FLAGS = -ffreestanding -Wall -Wextra \
              -fno-pic -fno-pie -Ivendors/limine -Isrc \
			  -Isrc/arch/$(TARGET_ARCH)/include

BASE_CFLAGS = $(COMMON_FLAGS) -std=gnu23 --embed-dir=assets
BASE_CXXFLAGS = $(COMMON_FLAGS) -std=c++23 -fno-exceptions -fno-rtti
BASE_ASFLAGS = $(COMMON_FLAGS) -std=gnu23 -c -x assembler-with-cpp

ifeq ($(TARGET_ARCH), x86)
    DEFAULT_BOOTLOADER := limine
    QEMU := qemu-system-x86_64
    ARCH_CFLAGS := --target=x86_64-elf -m64 -march=x86-64 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mcmodel=kernel
else ifeq ($(TARGET_ARCH), arm64)
    DEFAULT_BOOTLOADER := grub
    QEMU := qemu-system-aarch64
    ARCH_CFLAGS := --target=aarch64-elf -mgeneral-regs-only
else ifeq ($(TARGET_ARCH), arm32)
    DEFAULT_BOOTLOADER := grub
    QEMU := qemu-system-arm
    ARCH_CFLAGS := --target=arm-none-eabi -march=armv7-a -mfloat-abi=soft
else
    $(error "Unsupported or invalid TARGET_ARCH: '$(TARGET_ARCH)'. Valid options are: x86, arm64, arm32")
endif

BOOTLOADER ?= $(DEFAULT_BOOTLOADER)
BOOT_SRC = src/boot/$(BOOTLOADER).c

# ifeq ($(wildcard $(BOOT_SRC)),)
#     $(error "[FATAL] The bootloader '$(BOOTLOADER)' does not have an entry file at $(BOOT_SRC). It may not support $(TARGET_ARCH) or the file is missing.")
# endif

ifeq ($(PROFILE),debug)
    PROFILE_CFLAGS := -O0 -g
else ifeq ($(PROFILE),release)
    PROFILE_CFLAGS := -O2 -DNDEBUG
else
    $(error "Invalid PROFILE!")
endif

CFLAGS = $(BASE_CFLAGS) $(ARCH_CFLAGS) $(PROFILE_CFLAGS)
CXXFLAGS = $(BASE_CXXFLAGS) $(ARCH_CFLAGS) $(PROFILE_CFLAGS)
ASFLAGS = $(BASE_ASFLAGS) $(ARCH_CFLAGS) $(PROFILE_CFLAGS)

LDFLAGS = -T linker.ld -nostdlib -z max-page-size=0x1000

REQUIRED_BINS := $(CC) $(AS) xorriso qemu-system-x86_64 uv
$(foreach bin,$(REQUIRED_BINS),\
    $(if $(shell command -v $(bin) 2> /dev/null),,\
        $(error "Missing dependency: [$(bin)] is not installed or not in PATH.")))

BUILD_DIR = build/$(TARGET_ARCH)/$(PROFILE)
SRC_DIR = src
ISO_DIR = $(BUILD_DIR)/iso_root
TARGET_ELF = $(BUILD_DIR)/kernel.elf
TARGET_ISO = $(BUILD_DIR)/myos.iso
LIMINE_DIR = vendors/limine

CXX_SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
AS_SRCS := $(shell find $(SRC_DIR) -name '*.S')

CORE_SRCS := $(shell find $(SRC_DIR) -name '*.c' ! -path '$(SRC_DIR)/boot/*' ! -path '$(SRC_DIR)/arch/*')
ARCH_SRCS := $(shell find $(SRC_DIR)/arch/$(TARGET_ARCH) -name '*.c')
C_SRCS := $(CORE_SRCS) $(ARCH_SRCS) $(BOOT_SRC)

C_OBJS := $(C_SRCS:%.c=$(BUILD_DIR)/%.o)
CXX_OBJS := $(CXX_SRCS:%.cpp=$(BUILD_DIR)/%.o)
AS_OBJS := $(AS_SRCS:%.S=$(BUILD_DIR)/%.o)

OBJECTS := $(C_OBJS) $(CXX_OBJS) $(AS_OBJS)

.PHONY: all run clean

all: $(TARGET_ISO)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(TARGET_ELF): $(OBJECTS)
	$(CC) $(ARCH_CFLAGS) $(LDFLAGS) -o $@ $^

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
	$(QEMU) -cdrom $(TARGET_ISO) -m 256M -debugcon stdio

clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR) $(TARGET_ISO)
