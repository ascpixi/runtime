#include <stdarg.h>

#include "./host.h"
#include "./lib/debug-trap.h"  

int vflogf(FILE* file, const char* fmt, va_list args);

void __ryujit_pal_asserte_fail(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vflogf(jitstdout(), fmt, args);
    va_end(args);

    psnip_trap();
}
