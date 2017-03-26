#pragma once
#include <memory>
#include <forsage/memory_management/allocator.hpp>

namespace forsage
{

// make shared_ptr by using forsage_allocator
template <typename T, typename... ArgsT>
std::shared_ptr<T> make_shared(ArgsT&&... args)
{
    forsage_allocator<T> alloc;
    return std::allocate_shared<T>(alloc, std::forward<ArgsT>(args)...);
}

// make unique_ptr by using forsage_allocator
template <typename T, typename... ArgsT>
std::unique_ptr<T, forsage_delete<T>> make_unique(ArgsT&&... args)
{
    forsage_allocator<T> alloc;
    T* ptr = new(alloc.allocate(1)) T(std::forward<ArgsT>(args)...);
    return std::unique_ptr<T, forsage_delete<T>>(ptr);
}

} // namespace forsage