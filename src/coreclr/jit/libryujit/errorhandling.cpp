#include <csetjmp>
#include <functional>

#include "./errorhandling.h"
#include "./tls.h"

void try_finally(std::function<void()> block, std::function<void()> finally_handler)
{
    ryujit_get_tls()->exc_stack.push_back(exc_handler_t(finally_handler));
    block();

    // If an exception occurs, we shouldn't reach this point.
    ryujit_get_tls()->exc_stack.pop_back();
}

void try_catch(void* capture, void (*block)(void*), void (*handler)(int, void*))
{
    exc_jmp_buf jump = {};
    if (__builtin_setjmp(jump) > 0)
    {
        auto tls = ryujit_get_tls();
        tls->exc_catch_handler(tls->exc_val, tls->exc_catch_captures);
        return;
    }

    ryujit_get_tls()->exc_stack.push_back(exc_handler_t(exc_catch_handler_t{&jump, handler, capture}));

    block(capture);

    ryujit_get_tls()->exc_stack.pop_back();
}

void exc_throw(int val)
{
    auto tls     = ryujit_get_tls();
    tls->exc_val = val;

    while (!tls->exc_stack.empty())
    {
        exc_handler_t handler = tls->exc_stack[tls->exc_stack.size() - 1];
        tls->exc_stack.pop_back();

        if (handler.type == exc_handler_type_t::Catch)
        {
            tls->exc_catch_handler  = handler.catch_handler.body;
            tls->exc_catch_captures = handler.catch_handler.captures;
            __builtin_longjmp(*handler.catch_handler.buf, 1);
            return; // We shouldn't really reach this point...
        }
        else
        {
            auto& finally_handler = handler.finally_handler;
            if (!finally_handler)
            {
                ryujit_host_panic("'finally' block handler became empty");
            }

            finally_handler();
        }
    }

    // No (catch) handler is set-up! This is an unhandled exception!
    ryujit_host_panic("unhandled exception");
}
