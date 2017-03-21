#pragma once

#include <memory>

namespace react
{

template <typename T>
struct execution_element {};

template <typename T>
using execution_element_ptr = std::unique_ptr<execution_element<T>>;

} // namespace react