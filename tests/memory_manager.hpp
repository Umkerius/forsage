#pragma once
#include <mutex>
#include <unordered_map>

class memory_manager
{
public:
    void* allocate(size_t size);
    void deallocate(void* pointer);

    size_t get_allocated_objects_count() const;

private:
    std::unordered_map<void*, size_t> m_allocated_memory;
    mutable std::mutex m_mutex;
};