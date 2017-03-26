#pragma once
#include <forsage/memory_management/memory_management.hpp>

namespace forsage
{

// allocator for STL
template <typename T>
struct forsage_allocator
{
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    template <typename U>
    struct rebind { using other = forsage_allocator<U>; };

    forsage_allocator() = default;
    forsage_allocator(const forsage_allocator&) = default;
    template <typename U>
    forsage_allocator(const forsage_allocator<U>&) {};
    ~forsage_allocator() = default;

    pointer address(reference x) const { return &x; }
    const_pointer address(const_reference x) const { return &x; }

    pointer allocate(size_type n, const void* /*hint*/ = 0)
    {
        return reinterpret_cast<pointer>(memory_manager::allocate(n * sizeof(T)));
    }

    void deallocate(pointer p, size_type /*n*/)
    {
        memory_manager::deallocate(p);
    }

    size_type max_size() const { return std::numeric_limits<size_type>::max(); }

    template <typename U, typename... Args>
    void construct(U* p, Args&&... args)
    {
        new ((void*)p) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U* p)
    {
        p->~U();
    }
};

template <typename T1, typename T2>
bool operator==(const forsage_allocator<T1>& lhs, const forsage_allocator<T2>& rhs)
{
    return true;
}

template <typename T1, typename T2>
bool operator!=(const forsage_allocator<T1>& lhs, const forsage_allocator<T2>& rhs)
{
    return false;
}

template <typename T> 
struct forsage_delete
{
    constexpr forsage_delete() = default;
    void operator()(T* ptr) const
    {
        forsage_allocator<T> allocator;
        allocator.destroy(ptr);
        allocator.deallocate(ptr);
    }
};

} // namespace forsage