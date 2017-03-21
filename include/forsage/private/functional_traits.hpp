#pragma once
#include <tuple>
#include <type_traits>

namespace forsage
{
// Detect if T is callable object   
template<typename T>
struct is_callable;

template <typename T>
struct functional_traits;

namespace detail
{
struct func_traits_empty_impl
{
    using is_functional = std::false_type;
};

template <typename Ret, typename... Args>
struct func_traits_impl
{
    using is_functional = std::true_type;
    using return_type = Ret;
    using arguments = std::tuple<Args...>;
    using signature = Ret(Args...);
};

template <typename T>
struct func_traits : func_traits_empty_impl {};

template <typename Ret, typename... Args>
struct func_traits<Ret(Args...)> : func_traits_impl<Ret, Args...> {};

template <typename Ret, typename... Args>
struct func_traits<Ret(*)(Args...)> : func_traits_impl<Ret, Args...> {};

template <typename Class, typename Ret, typename... Args>
struct func_traits<Ret(Class::*)(Args...)> : func_traits_impl<Ret, Args...> {};

template <typename Class, typename Ret, typename... Args>
struct func_traits<Ret(Class::*)(Args...) const> : func_traits_impl<Ret, Args...> {};

template <typename T>
struct func_traits_t : func_traits<typename T::type> {};

struct base_result_wrapper {};

template <typename T>
struct result_wrapper : base_result_wrapper 
{
    using type = T;
};
template <typename T>
using is_result_wrapper = std::is_base_of<base_result_wrapper, T>;

template <typename T>
auto func_selector(typename std::enable_if<std::is_function<T>::value>::type* = nullptr)
    -> result_wrapper<typename std::conditional<std::is_function<T>::value, T*, T>::type>;

template <typename T>
auto func_selector(typename std::enable_if<std::is_member_function_pointer<T>::value>::type* = nullptr)
    -> result_wrapper<T>;

template <typename T>
auto func_selector(typename std::enable_if<std::is_member_function_pointer<decltype(&std::decay<T>::type::operator())>::value>::type* = nullptr)
    -> result_wrapper<decltype(&std::decay<T>::type::operator())>;

template<typename T, typename = void>
struct is_callable_impl : std::false_type {};

template<typename T>
struct is_callable_impl<T, typename std::enable_if<
                                         detail::is_result_wrapper<
                                            decltype(detail::func_selector<T>())>::value>::type> : std::true_type {};

template <typename T, typename = void>
struct functional_traits_impl : func_traits_empty_impl {};

template <typename T>
struct functional_traits_impl<T, typename std::enable_if<is_callable<T>::value>::type> : func_traits_t<decltype(func_selector<T>())> {};

} // namespace detail

template<typename T>
struct is_callable : detail::is_callable_impl<T> {};

template <typename T>
struct functional_traits : detail::functional_traits_impl<T> {};

} // namespace forsage
