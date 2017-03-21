#pragma once

#include <type_traits>
#include <react/execution_chain.hpp>
#include <react/private/functional_traits.hpp>

namespace react
{
namespace detail
{
template <typename T>
auto filter(T&& func, std::true_type) 
{
    using return_type = functional_traits<T>::return_type;
    using arguments = functional_traits<T>::arguments;

    static_assert(std::is_convertible<return_type, bool>::value, 
                  "Function must have convertible to bool return type");
    static_assert(std::tuple_size<arguments>::value == 1,
                  "Function must have only one argument");

    using arg_type = std::decay<std::tuple_element<0, arguments>::type>::type;
	return execution_element_ptr<arg_type>();
}

template <typename T>	
execution_element_ptr<T> filter(T&& value, std::false_type)
{ 
	return nullptr; 
}

} // namespace detail
} // namespace react