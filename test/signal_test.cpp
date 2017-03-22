#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>

#include <string>
#include <vector>
#include <forsage/signal.hpp>

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

BOOST_AUTO_TEST_CASE(DirectSignalToFunctorConnectionTest)
{
    forsage::signal<int> src;

    int value = 0;
    src >> forsage::slot(setter<int>(value));

    src(42);
    BOOST_CHECK(value == 42);
}

BOOST_AUTO_TEST_CASE(DirectSignalToFunctorConnectionTest2)
{
    forsage::signal<int> src;

    int value = 0;
    src >> forsage::slot(setter<int>(value));

    src(42);
    BOOST_CHECK(value == 42);
}

BOOST_AUTO_TEST_CASE(DirectSignalToFunctorConnectionTest3)
{
    forsage::signal<int> src;

    double value = 0.0;
    src >> forsage::slot(setter<double>(value));

    src(42);
    BOOST_CHECK(value == 42.0);
}

BOOST_AUTO_TEST_CASE(DirectSignalToSignalConnectionTest)
{
    forsage::signal<int> src;
    forsage::signal<int> dst;
    src >> dst;

    int value = 0;
    dst >> forsage::slot(setter<int>(value));

    src(42);
    BOOST_CHECK(value == 42);
}

BOOST_AUTO_TEST_CASE(DirectSignalToSignalConnectionTest2)
{
    forsage::signal<double> src;
    forsage::signal<int> dst;
    src >> dst;

    int value = 0;
    dst >> forsage::slot(setter<int>(value));

    src(42.0);
    BOOST_CHECK(value == 42);
}

BOOST_AUTO_TEST_CASE(DirectSignalToSignalConnectionTest3)
{
    forsage::signal<double> src;
    forsage::signal<int> dst;

    int value = 0;
    src >> dst;

    dst >> forsage::slot(setter<int>(value));
    src(42.0);
    BOOST_CHECK(value == 42);
}

BOOST_AUTO_TEST_CASE(ChainedConnectionTest)
{
    forsage::signal<int> src;

    int value = 0;
    src >> multiplier >> forsage::slot(setter<int>(value));

    src(42);
    BOOST_CHECK(value == multiplier(42));
}

BOOST_AUTO_TEST_CASE(ChainedConnectionTest2)
{
    forsage::signal<int> src;
    forsage::signal<int> dst;
    src >> multiplier >> dst;

    int value = 0;
    dst >> forsage::slot(setter<int>(value));
    src(42);
    BOOST_CHECK(value == multiplier(42));
}

BOOST_AUTO_TEST_CASE(ChainedConnectionTest3)
{
    forsage::signal<int> src;
    forsage::signal<int> dst;

    int value = 0;
    src >> multiplier >> dst;

    dst >> forsage::slot(setter<int>(value));
    src(42);
    BOOST_CHECK(value == multiplier(42));
}

BOOST_AUTO_TEST_CASE(ChainedConnectionTest4)
{
    int value = 0;

    forsage::signal<int> src;
    src >> multiplier
        >> divider
        >> forsage::slot(setter<int>(value));

    src(42);
    BOOST_CHECK(value == divider(multiplier(42)));
}

BOOST_AUTO_TEST_CASE(ChainedConnectionTest5)
{
    int value = 0;

    forsage::signal<int> src;
    forsage::signal<int> dst;
    src >> multiplier
        >> divider
        >> dst;

    dst >> forsage::slot(setter<int>(value));
    src(42);
    BOOST_CHECK(value == divider(multiplier(42)));
}

BOOST_AUTO_TEST_CASE(LongChainedConnectionTest)
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
    BOOST_CHECK(value == 42);
}

// Uncomment after fixing of all tests above
/*BOOST_AUTO_TEST_CASE(LongChainedConnectionTest2)
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
    BOOST_CHECK(value == 42);
}*/