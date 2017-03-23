#pragma once

#include <memory>
#include <list>
#include <forsage/private/functional_traits.hpp>

namespace forsage
{
template <typename Func>
constexpr bool check_func()
{
    using traits = functional_traits<Func>;
    static_assert(traits::is_functional::value, "Object must be callable (function or functor)");
    static_assert(std::tuple_size<typename traits::arguments>::value == 1,
                  "Only single argument signals and functional objects are supported");
    return true; // constexpr func must return value
}

struct connection_if
{
    virtual ~connection_if() = default;
};

template <typename T>
struct connection : public connection_if 
{
    connection(std::function<void(T)>&& func = nullptr) 
        : m_entry_func(std::move(func)) {}

    void call(const T& value) 
    {
        m_entry_func(value);
    }

    std::function<void(T)> m_entry_func;
};

template <typename T>
using connection_ptr = std::shared_ptr<connection<T>>;

template <typename T>
using connection_weak_ptr = std::weak_ptr<connection<T>>;

template <typename T>
struct signal
{
    void operator()(const T& value) 
    {
        for (auto& connection : m_connections)
        {
            connection->call(value);
        }
    }
    std::list<connection_ptr<T>> m_connections;
};

template <typename T>
struct slot_t
{
    slot_t(std::function<void(T)> func) : m_func(std::move(func)) {}
    std::function<void(T)> m_func;
};

template <typename... args>
struct connection_builder
{
    std::tuple<args...> call_chain;
};

struct end_point {};

template <typename T, typename Y>
end_point operator>>(signal<T>& src, signal<Y>& dst)
{
    static_assert(std::is_convertible<T, Y>::value, 
                  "Trying to connect signals with non-convertible types");

    auto conn_func = [&dst](const T& val)
    {
        dst(static_cast<Y>(val));
    };
    src.m_connections.emplace_back(std::make_shared<connection<T>>(std::move(conn_func)));

    return end_point();
}

template <typename T, typename Y>
end_point operator>>(signal<T>& src, slot_t<Y>&& dst)
{
    return end_point();
}

template <typename T, typename Func>
auto operator>>(signal<T>& src, Func&& dst)
{
    check_func<Func>();

    using traits = functional_traits<Func>;
    using arg_type = typename std::tuple_element<0, typename traits::arguments>::type;

    return connection_builder<std::function<void(arg_type)>>();
}

template <typename T, typename... args>
void operator>>(connection_builder<args...>&& src, slot_t<T>&& dst)
{
}

template <typename T, typename... args>
end_point operator>>(connection_builder<args...>&& src, signal<T>& dst)
{
    return end_point();
}

template <typename Func, typename... args>
auto operator>>(connection_builder<args...>&& src, Func&& func/*, std::enable_if<is_callable<Func>::value>::type* = nullptr*/) -> decltype(auto)
{
    return connection_builder<args...>();
}

template <typename Func>
auto slot(Func&& func)
{
    check_func<Func>();

    using traits = functional_traits<Func>;
    using arg_type = typename std::tuple_element<0, typename traits::arguments>::type;

    return slot_t<arg_type>(std::forward<Func>(func));
}

} // namespace forsage