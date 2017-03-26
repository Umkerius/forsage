#include "memory_leak_detector.hpp" 
#include <forsage/memory_management/memory_management.hpp>

memory_manager* g_memory_manager = nullptr;

memory_leak_detector::memory_leak_detector()
{
    g_memory_manager = &m_mem_manager;
    forsage::memory_manager::override_mem_functions(allocate_proxy, deallocate_proxy);
}

memory_leak_detector::~memory_leak_detector()
{
    forsage::memory_manager::reset_mem_functions();
    g_memory_manager = nullptr;
}

void* memory_leak_detector::allocate_proxy(size_t size)
{
    return g_memory_manager->allocate(size);
}

void memory_leak_detector::deallocate_proxy(void* ptr)
{
    return g_memory_manager->deallocate(ptr);
}

void memory_leak_detector::OnTestStart(const ::testing::TestInfo& test_info)
{
    m_last_allocation_count = m_mem_manager.get_allocated_objects_count();
}

void memory_leak_detector::OnTestEnd(const ::testing::TestInfo& test_info)
{
    size_t allocation_count = m_mem_manager.get_allocated_objects_count();
    if (allocation_count != m_last_allocation_count)
    {
        FAIL() << "Memory leak detected.";
    }
}