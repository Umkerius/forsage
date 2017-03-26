#pragma once
#include "memory_manager.hpp" 
#include <cstdint>
#include <gtest/gtest.h>

class memory_leak_detector : public ::testing::EmptyTestEventListener
{
public:
    memory_leak_detector();
    ~memory_leak_detector();

    static void* allocate_proxy(size_t size);
    static void deallocate_proxy(void* ptr);

    // EmptyTestEventListener implementation
    void OnTestStart(const ::testing::TestInfo& test_info) override;
    void OnTestEnd(const ::testing::TestInfo& test_info) override;

private:
    memory_manager m_mem_manager;
    size_t m_last_allocation_count = 0;
};