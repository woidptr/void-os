# void-os

Operating system made with c23, using the limine bootloader.

## Requirements

To build and emulate the OS, you will need a Unix-like environment (Linux, macOS, or WSL) with the following tools installed:

### Core Build Tools
* **`make`**: GNU Make for executing the build system.
* **`nasm`**: The Netwide Assembler, used for compiling raw assembly files.
* **`x86_64-elf-gcc`**: The cross-compiler used to build the C kernel, without linking to host OS or standard library.
* **`xorriso`**: Required to pack the compiled kernel and Limine bootloader into a bootable `.iso` image.

### Emulation
* **`qemu-system-x86_64`**: The QEMU emulator used to test the OS without flashing it to physical hardware.

### Installation Commands

**macOS (via Homebrew)**
```bash
brew install make nasm x86_64-elf-gcc xorriso qemu
```

## Building

```sh
make run
```
