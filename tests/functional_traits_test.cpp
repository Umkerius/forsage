#include <type_traits>
#include <functional>

#include <forsage/private/functional_traits.hpp>
#include <gtest/gtest.h>

#define FUNCTIONAL_TRAITS_TEST(case_name) TEST(FunctionalTraitsTest, case_name)

using forsage::functional_traits;

int var = 42;

int function() { return 0; }
int& ref_function(void*, size_t) { return var; }
const int& cref_function(void*, size_t&) { return var; }

struct functional_object
{
    int operator()(int) { return 0; }
};

struct functional_object_const
{
    int operator()(int) const { return 0; }
};

FUNCTIONAL_TRAITS_TEST(FunctionalTraitsTest)
{
    decltype(&functional_object::operator()) method;
    decltype(&functional_object_const::operator()) const_method;
    auto lambda = [] {};
    method = nullptr;
    const_method = nullptr;

    // Basic test
    ASSERT_TRUE(forsage::is_callable<decltype(function)>::value);
    ASSERT_FALSE(forsage::is_callable<decltype(42)>::value);
    ASSERT_TRUE(forsage::is_callable<decltype(method)>::value);
    ASSERT_TRUE(forsage::is_callable<decltype(const_method)>::value);

    // functional_traits test
    ASSERT_TRUE(functional_traits<decltype(function)>::is_functional::value);
    ASSERT_FALSE(functional_traits<decltype(42)>::is_functional::value);
    ASSERT_TRUE(functional_traits<decltype(ref_function)>::is_functional::value);
    ASSERT_TRUE(functional_traits<decltype(cref_function)>::is_functional::value);
    ASSERT_TRUE(functional_traits<decltype(method)>::is_functional::value);
    ASSERT_TRUE(functional_traits<decltype(const_method)>::is_functional::value);
    ASSERT_TRUE(functional_traits<functional_object>::is_functional::value);
    ASSERT_TRUE(functional_traits<functional_object_const>::is_functional::value);
    ASSERT_TRUE(functional_traits<std::function<int()>>::is_functional::value);
    ASSERT_TRUE(functional_traits<decltype(lambda)>::is_functional::value);
}

FUNCTIONAL_TRAITS_TEST(FunctionTraitsTest)
{
    using traits = functional_traits<decltype(function)>;

    ASSERT_TRUE(traits::is_functional::value);
    ASSERT_TRUE((std::is_same<traits::return_type, int>::value));
    ASSERT_EQ(std::tuple_size<traits::arguments>::value, 0u);
}

FUNCTIONAL_TRAITS_TEST(RefFunctionTraitsTest)
{
    using traits = functional_traits<decltype(ref_function)>;

    ASSERT_TRUE(traits::is_functional::value);
    ASSERT_TRUE((std::is_same<traits::return_type, int&>::value));
    ASSERT_TRUE((std::is_same<traits::arguments, std::tuple<void*, size_t>>::value));
    ASSERT_EQ(std::tuple_size<traits::arguments>::value, 2u);
}

FUNCTIONAL_TRAITS_TEST(CRefFunctionTraitsTest)
{
    using traits = functional_traits<decltype(cref_function)>;

    ASSERT_TRUE(traits::is_functional::value);
    ASSERT_TRUE((std::is_same<traits::return_type, const int&>::value));
    ASSERT_TRUE((std::is_same<traits::arguments, std::tuple<void*, size_t&>>::value));
    ASSERT_EQ(std::tuple_size<traits::arguments>::value, 2u);
}

template <typename Func>
auto universal_ref_func_checker(Func&&)
{
    using traits = functional_traits<Func>;
    ASSERT_TRUE(traits::is_functional::value);
}

FUNCTIONAL_TRAITS_TEST(UniversalRefFunctionTraitsTest)
{
    auto foo = [](int) {};
    const auto const_foo = [](int) {};
    universal_ref_func_checker(foo);
    universal_ref_func_checker(const_foo);
    universal_ref_func_checker([](int) {});
}