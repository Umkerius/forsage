#include "memory_leak_detector.hpp"
#include <gtest/gtest.h>

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    
    // Add memory leak detector
    ::testing::TestEventListeners& listeners = 
        ::testing::UnitTest::GetInstance()->listeners();
    listeners.Append(new memory_leak_detector);

    return RUN_ALL_TESTS();
}