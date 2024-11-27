#pragma once

/// These functions must be implemented by the consumers of the static library.

// Allocate memory of the given size in bytes.
//      Passed to: `ICorJitHost::allocateMemory`
extern "C" void* ryujit_host_alloc(size_t size);

// Frees memory previous obtained by a call to `ryujit_alloc`.
//      Passed to: `ICorJitHost::freeMemory`
extern "C" void ryujit_host_free(void* block);

// Returns an integer config value for the given key, if any exists.
//      Passed to: `ICorJitHost::getIntConfigValue`
extern "C" int ryujit_host_get_int_cfgval(
    const char* name,
    int defaultValue
);

// Return a string config value for the given key, if any exists.
//      Passed to: `ICorJitHost::getStringConfigValue`
extern "C" const char* ryujit_host_get_string_cfgval(const char* name);

// Free a string ConfigValue returned by the runtime. JITs using the getStringConfigValue
// query are required to return the string values to the runtime for deletion.
// This avoids leaking the memory in the JIT.
//      Passed to: `ICorJitHost::freeStringConfigValue`
extern "C" void ryujit_host_free_string_cfgval(const char* value);

// Allocate memory slab of the given size in bytes. The host is expected to pool
// these for good performance.
//      Passed to: `ICorJitHost::allocateSlab`
extern "C" void* ryujit_host_alloc_slab(size_t size, size_t* pActualSize);

// Free memory slab of the given size in bytes.
//      Passed to: `ICorJitHost::freeSlab`
extern "C" void ryujit_host_free_slab(void* slab, size_t actualSize);