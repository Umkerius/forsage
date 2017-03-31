#pragma once
#include <cstddef>

namespace forsage
{
struct memory_manager
{
    using alloc_func = void*(*)(std::size_t);
    using dealloc_func = void(*)(void*);

    static void* allocate(std::size_t bytes);
    static void deallocate(void* ptr);

    // override functions
    static void override_mem_functions(alloc_func alloc, dealloc_func dealloc);
    // reset to defaults
    static void reset_mem_functions();
};
} // namespace forsage