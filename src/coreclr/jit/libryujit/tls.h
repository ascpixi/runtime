#pragma once

#include "./host.h"
#include "./errorhandling.h"

typedef struct
{
    // Data determined and entirely owned by RyuJIT.
    void* jit_data;

    // Provides try-catch-finally block data.
    exc_stack_t exc_stack;

    // Provides the value (`__errc`) of the currently handled exception.
    int exc_val;

    // When a `catch` setjmp target is invoked, this field informs said target
    // of the handler that it needs to call.
    void (*exc_catch_handler)(int, void*);

    // When a `catch` setjmp target is invoked, this field informs said target
    // of the captures to provide to the catch handler.
    void* exc_catch_captures;
} ryujit_tls_t;

// Gets data exclusive to each calling thread.
ryujit_tls_t* ryujit_get_tls();
