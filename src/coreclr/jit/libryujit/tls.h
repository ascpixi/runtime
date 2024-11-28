#pragma once

#include "libryujit/host.h"
#include "libryujit/errorhandling.h"

typedef struct {
    // Data determined and entirely owned by RyuJIT.
    void* jit_data;

    // Provides try-catch-finally block data.
    exc_stack_t exc_stack;

    // Provides the value (`__errc`) of the currently handled exception.
    int exc_val;
} ryujit_tls_t;

// Gets data exclusive to each calling thread.
ryujit_tls_t* ryujit_get_tls();
