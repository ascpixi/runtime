#include <csetjmp>
#include <functional>

#include "libryujit/errorhandling.h"
#include "libryujit/tls.h"

void try_finally(std::function<void()> block, std::function<void()> finally_handler)
{
    ryujit_get_tls()->exc_stack.push_back(exc_handler_t(finally_handler));
    block();

    // If an exception occurs, we shouldn't reach this point.
    ryujit_get_tls()->exc_stack.pop_back();
}

void try_catch(std::function<void()> block, std::function<void(int)> handler)
{
    std::jmp_buf jump;
    if (setjmp(jump) > 0)
    {
        // The handler automatically gets
        handler(ryujit_get_tls()->exc_val);
        return;
    }

    ryujit_get_tls()->exc_stack.push_back(exc_handler_t(exc_catch_handler_t{&jump}));
    block();
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
            longjmp(*handler.catch_handler.buf, 1);
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
