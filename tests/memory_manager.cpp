#include "memory_manager.hpp" 

void* memory_manager::allocate(size_t size)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    void* ptr = ::malloc(size);
    m_allocated_memory[ptr] = size;
    return ptr;
}

void memory_manager::deallocate(void* pointer)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    ::free(pointer);
    m_allocated_memory.erase(pointer);
}

size_t memory_manager::get_allocated_objects_count() const
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_allocated_memory.size();
}