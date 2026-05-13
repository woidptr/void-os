# void-os

Operating system made with c23, using the limine bootloader.

## Requirements

To build and emulate the OS, you will need a Unix-like environment (Linux, macOS, or WSL) with the following tools installed:

### Core Build Tools

* **`make`** or `shell`: GNU Make or shell for launching the build script.
* **`python3`**: For the build script.
* **`clang`**: The cross-compiler used to build the kernel, without linking to host OS or standard library.
* **`xorriso`**: Required to pack the compiled kernel and Limine bootloader into a bootable `.iso` image.

### Emulation

* **`qemu-system-x86_64`**: The QEMU emulator used to test the OS without flashing it to physical hardware.

## Build Interfaces

This project uses a unified Python-based build engine. For convenience, there are two identical interfaces to the engine depending on your preferred workflow. Both scripts accept the same arguments and provide full help documentation.

### Unified Command Interface

You can interact with the build system through either make or the shell wrapper. Choose the one that fits your muscle memory:

**Using make:**

```bash
make --help
```

**Using the shell script:**

```bash
./build.sh --help
```
