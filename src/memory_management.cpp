#include <forsage/memory_management/memory_management.hpp>

#include <memory>

namespace forsage
{

memory_manager::alloc_func get_default_alloc_func() { return ::malloc; }
memory_manager::dealloc_func get_default_dealloc_func() { return ::free; }

class memory_manager_impl
{
public:
    memory_manager_impl()
    {
        reset_mem_functions();
    }

    void* allocate(size_t bytes)
    {
        return m_allocator(bytes);
    }

    void deallocate(void* ptr)
    {
        m_deallocator(ptr);
    }

    void override_mem_functions(memory_manager::alloc_func alloc, 
                                memory_manager::dealloc_func dealloc)
    {
        m_allocator = alloc;
        m_deallocator = dealloc;
    }

    void reset_mem_functions()
    {
        m_allocator = get_default_alloc_func();
        m_deallocator = get_default_dealloc_func();
    }

private:
    memory_manager::alloc_func m_allocator;
    memory_manager::dealloc_func m_deallocator;
};

memory_manager_impl& get_memory_manager()
{
    static memory_manager_impl mem_man;
    return mem_man;
}

void* memory_manager::allocate(size_t bytes)
{
    return get_memory_manager().allocate(bytes);
}

void memory_manager::deallocate(void* ptr)
{
    get_memory_manager().deallocate(ptr);
}

void memory_manager::override_mem_functions(memory_manager::alloc_func alloc, 
                                            memory_manager::dealloc_func dealloc)
{
    get_memory_manager().override_mem_functions(alloc, dealloc);
}

void memory_manager::reset_mem_functions()
{
    get_memory_manager().reset_mem_functions();
}

} // namespace forsage