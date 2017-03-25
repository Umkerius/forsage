#include <string>
#include <vector>

#include <forsage/signal.hpp>
#include <gtest/gtest.h>

#define SIGNAL_TEST(case_name) TEST(SignalTest, case_name)

// different functional types for testing purpose
template <typename T>
struct setter
{
    setter(T& value) : m_value(value) {}
    void operator()(const T& value)
    {
        m_value = value;
    }
    T& m_value;
};

auto multiplier = [](int value) { return value * 2; };
int divider(int value) { return value / 2; }

// TODO: try to use stoi directly
int string2int(const std::string& str)
{
    return std::stoi(str);
}

SIGNAL_TEST(DirectSignalToFunctorConnectionTest)
{
    forsage::signal<int> src;

    int value = 0;
    src >> forsage::slot(setter<int>(value));

    src(42);
    ASSERT_EQ(value, 42);
}

SIGNAL_TEST(DirectSignalToFunctorConnectionTest2)
{
    forsage::signal<int> src;

    int value = 0;
    src >> forsage::slot(setter<int>(value));

    src(42);
    ASSERT_EQ(value, 42);
}

SIGNAL_TEST(DirectSignalToFunctorConnectionTest3)
{
    forsage::signal<int> src;

    double value = 0.0;
    src >> forsage::slot(setter<double>(value));

    src(42);
    ASSERT_EQ(value, 42.0);
}

SIGNAL_TEST(DirectSignalToSignalConnectionTest)
{
    forsage::signal<int> src;
    forsage::signal<int> dst;
    src >> dst;

    int value = 0;
    dst >> forsage::slot(setter<int>(value));

    src(42);
    ASSERT_EQ(value, 42);
}

SIGNAL_TEST(DirectSignalToSignalConnectionTest2)
{
    forsage::signal<double> src;
    forsage::signal<int> dst;
    src >> dst;

    int value = 0;
    dst >> forsage::slot(setter<int>(value));

    src(42.0);
    ASSERT_EQ(value, 42);
}

SIGNAL_TEST(DirectSignalToSignalConnectionTest3)
{
    forsage::signal<double> src;
    forsage::signal<int> dst;

    int value = 0;
    src >> dst;

    dst >> forsage::slot(setter<int>(value));
    src(42.0);
    ASSERT_EQ(value, 42);
}

SIGNAL_TEST(ChainedConnectionTest)
{
    forsage::signal<int> src;

    int value = 0;
    src >> multiplier >> forsage::slot(setter<int>(value));

    src(42);
    ASSERT_EQ(value, multiplier(42));
}

SIGNAL_TEST(ChainedConnectionTest2)
{
    forsage::signal<int> src;
    forsage::signal<int> dst;
    src >> multiplier >> dst;

    int value = 0;
    dst >> forsage::slot(setter<int>(value));
    src(42);
    ASSERT_EQ(value, multiplier(42));
}

SIGNAL_TEST(ChainedConnectionTest3)
{
    forsage::signal<int> src;
    forsage::signal<int> dst;

    int value = 0;
    src >> multiplier >> dst;

    dst >> forsage::slot(setter<int>(value));
    src(42);
    ASSERT_EQ(value, multiplier(42));
}

SIGNAL_TEST(ChainedConnectionTest4)
{
    int value = 0;

    forsage::signal<int> src;
    src >> multiplier
        >> divider
        >> forsage::slot(setter<int>(value));

    src(42);
    ASSERT_EQ(value, divider(multiplier(42)));
}

SIGNAL_TEST(ChainedConnectionTest5)
{
    int value = 0;

    forsage::signal<int> src;
    forsage::signal<int> dst;
    src >> multiplier
        >> divider
        >> dst;

    dst >> forsage::slot(setter<int>(value));
    src(42);
    ASSERT_EQ(value, divider(multiplier(42)));
}

SIGNAL_TEST(LongChainedConnectionTest)
{
    int value = 0;

    forsage::signal<int> src;
    src >> multiplier
        >> multiplier
        >> multiplier
        >> divider
        >> divider
        >> divider
        >> forsage::slot(setter<int>(value));

    src(42);
    ASSERT_EQ(value, 42);
}

// Uncomment after fixing of all tests above
/*SIGNAL_TEST(LongChainedConnectionTest2)
{
    int value = 0;

    forsage::signal<std::string> src;
    src >> string2int
    >> multiplier
    >> multiplier
    >> multiplier
    >> divider
    >> divider
    >> divider
    >> forsage::slot(setter<int>(value));

    src("42");
    ASSERT_EQ(value, 42);
}*/