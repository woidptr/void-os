# $@ -> target
# $^ -> prerequisites


# Definition of compiler and linker
CC = i686-elf-gcc
AS = i686-elf-as

# Definitions of cmd flags
CFLAGS = -std=gnu23 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib -lgcc

# Definition of build dir
BUILD_DIR = build
TARGET = $(BUILD_DIR)/myos.bin
OBJS = $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o
SOURCE_DIR = src

all: $(BUILD_DIR)/myos.bin

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	$(AS) $< -o $@

run: $(TARGET)
	qemu-system-i386 -kernel $(TARGET)

# Clean the build files
clean:
	rm -rf $(BUILD_DIR)
