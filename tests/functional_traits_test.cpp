#include <type_traits>
#include <functional>

#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>

#include <forsage/private/functional_traits.hpp>

using forsage::functional_traits;

int var = 42;

int function() { return 0; }
int& ref_function(void* ptr, size_t size) { return var; }
const int& cref_function(void* ptr, size_t& size) { return var; }

struct functional_object
{
    int operator()(int) { return 0; }
};

struct functional_object_const
{
    int operator()(int) const { return 0; }
};

BOOST_AUTO_TEST_CASE(FunctionalTraitsTest)
{
    decltype(&functional_object::operator()) method;
    decltype(&functional_object_const::operator()) const_method;
    auto lambda = [] {};
    method = nullptr;
    const_method = nullptr;

    // Basic test
    BOOST_CHECK(forsage::is_callable<decltype(function)>::value == true);
    BOOST_CHECK(forsage::is_callable<decltype(42)>::value == false);
    BOOST_CHECK(forsage::is_callable<decltype(method)>::value == true);
    BOOST_CHECK(forsage::is_callable<decltype(const_method)>::value == true);

    // functional_traits test
    BOOST_CHECK(functional_traits<decltype(function)>::is_functional::value == true);
    BOOST_CHECK(functional_traits<decltype(42)>::is_functional::value == false);
    BOOST_CHECK(functional_traits<decltype(ref_function)>::is_functional::value == true);
    BOOST_CHECK(functional_traits<decltype(cref_function)>::is_functional::value == true);
    BOOST_CHECK(functional_traits<decltype(method)>::is_functional::value == true);
    BOOST_CHECK(functional_traits<decltype(const_method)>::is_functional::value == true);
    BOOST_CHECK(functional_traits<functional_object>::is_functional::value == true);
    BOOST_CHECK(functional_traits<functional_object_const>::is_functional::value == true);
    BOOST_CHECK(functional_traits<std::function<int()>>::is_functional::value == true);
    BOOST_CHECK(functional_traits<decltype(lambda)>::is_functional::value == true);
}

BOOST_AUTO_TEST_CASE(FunctionTraitsTest)
{
    using traits = functional_traits<decltype(function)>;

    BOOST_CHECK(traits::is_functional::value == true);
    BOOST_CHECK((std::is_same<traits::return_type, int>::value == true));
    BOOST_CHECK(std::tuple_size<traits::arguments>::value == 0);
}

BOOST_AUTO_TEST_CASE(RefFunctionTraitsTest)
{
    using traits = functional_traits<decltype(ref_function)>;

    BOOST_CHECK(traits::is_functional::value == true);
    BOOST_CHECK((std::is_same<traits::return_type, int&>::value == true));
    BOOST_CHECK(std::tuple_size<traits::arguments>::value == 2);
    BOOST_CHECK((std::is_same<traits::arguments, std::tuple<void*, size_t>>::value == true));
}

BOOST_AUTO_TEST_CASE(CRefFunctionTraitsTest)
{
    using traits = functional_traits<decltype(cref_function)>;

    BOOST_CHECK(traits::is_functional::value == true);
    BOOST_CHECK((std::is_same<traits::return_type, const int&>::value == true));
    BOOST_CHECK(std::tuple_size<traits::arguments>::value == 2);
    BOOST_CHECK((std::is_same<traits::arguments, std::tuple<void*, size_t&>>::value == true));
}

template <typename Func>
auto universal_ref_func_checker(Func&& func)
{
    using traits = functional_traits<Func>;
    BOOST_CHECK(traits::is_functional::value == true);
}

BOOST_AUTO_TEST_CASE(UniversalRefFunctionTraitsTest)
{
    auto foo = [](int) {};
    const auto const_foo = [](int) {};
    universal_ref_func_checker(foo);
    universal_ref_func_checker(const_foo);
    universal_ref_func_checker([](int) {});
}