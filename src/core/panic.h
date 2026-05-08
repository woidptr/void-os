#pragma once
#include <stdint.h>
#include "core/hal/ints.h"

struct kernel_ctx;

[[noreturn]] void kernel_panic(struct kernel_ctx* kctx, const char* fmt, ...);