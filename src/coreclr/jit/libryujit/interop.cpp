#include <new>

#include "corjithost.h"
#include "corjit.h"
#include "libryujit/interopjitinfo.h"
#include "libryujit/host.h"

#ifndef DLLEXPORT
    #define DLLEXPORT
#endif // !DLLEXPORT

class RyuJitHost : public ICorJitHost
{
public:
    void* allocateMemory(size_t size) override {
        return ryujit_host_alloc(size);
    }

    void freeMemory(void* block) override {
        ryujit_host_free(block);
    }

    int getIntConfigValue(const char* name, int defaultValue) override {
        return ryujit_host_get_int_cfgval(name, defaultValue);
    }

    const char* getStringConfigValue(const char* name) override {
        return ryujit_host_get_string_cfgval(name);
    }

    void freeStringConfigValue(const char* value) override {
        ryujit_host_free_string_cfgval(value);
    }

    void* allocateSlab(size_t size, size_t* pActualSize) override {
        return ryujit_host_alloc_slab(size, pActualSize);
    }

    void freeSlab(void* slab, size_t actualSize) override {
        ryujit_host_free_slab(slab, actualSize);
    }
};

// Gets the global `RyuJitHost` instance.
extern "C" DLLEXPORT RyuJitHost* ryujit_get_host() {
    static RyuJitHost instance;
    return &instance;
}

// Compiles the given method, given a handle (object pointer) to an `ICorJitCompiler`
// object. In most cases, a `ICorJitCompiler*` should be treated like an opaque handle.
//
// You may obtain a `ICorJitInfo*` via the `ryujit_create_jitinfo` function.
extern "C" DLLEXPORT CorJitResult ryujit_compile_method(
    ICorJitCompiler* self,
    ICorJitInfo*                    comp,               /* IN */
    struct CORINFO_METHOD_INFO*     info,               /* IN */
    unsigned /* code:CorJitFlag */  flags,              /* IN */
    uint8_t**                       nativeEntry,        /* OUT */
    uint32_t*                       nativeSizeOfCode    /* OUT */
) {
    return self->compileMethod(
        comp,
        info,
        flags,
        nativeEntry,
        nativeSizeOfCode
    );
}

// Creates a `ICorJitInfo` object which can be used to create an interface between
// the invoking execution environment (EE) with the JIT.
// Will use `ryujit_host_alloc` to allocate memory for the object.
//
// Parameters:
// - `self`: an arbitrary pointer to pass onto all methods inside `methods`.
// - `reportNotImplemented`: a function to call if a method is called that has its pointer set to `NULL` inside `methods`.
// - `methods`: a table of methods the JIT is allowed to invoke.
extern "C" DLLEXPORT ICorJitInfo* ryujit_create_jitinfo(
    void* self,
    void (*reportNotImplemented)(const char* methodName),
    JitInfoMethods* methods
) {
    void* ptr = ryujit_host_alloc(sizeof(UniversalJitInfo));

    return ::new(ptr) UniversalJitInfo(
        self,
        reportNotImplemented,
        methods
    );
}

