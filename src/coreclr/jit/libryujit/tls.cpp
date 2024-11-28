#include "libryujit/tls.h"
#include "libryujit/host.h"
#include "libryujit/errorhandling.h"

ryujit_tls_t* ryujit_get_tls() {
    auto data = (ryujit_tls_t*)ryujit_host_get_tls();
    if (data == nullptr) {
        data = (ryujit_tls_t*)ryujit_host_alloc(sizeof(ryujit_tls_t));
        if (data == nullptr) {
            ryujit_host_panic("couldn't allocate memory for TLS");
            return nullptr;
        }

        ryujit_host_set_tls(data);

        new (&data->exc_stack) exc_stack_t();
    }

    return data;
}
