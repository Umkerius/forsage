#pragma once

#include <utility>
#include <react/execution_chain.hpp>
#include <react/private/algorithm/filter.hpp>
#include <react/private/functional_traits.hpp>

namespace react
{
template <typename T>
auto filter(T&& arg)
{
	return detail::filter<T>(std::forward<T>(arg), typename functional_traits<T>::is_functional());
}
} // namespace react