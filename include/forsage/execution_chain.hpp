#pragma once

#include <utility>
#include <react/execution_element.hpp>
#include <react/source.hpp>
#include <react/receiver.hpp>

namespace react
{

struct execution_chain 
{
    void reset();
    bool empty() const;
};

template <typename T>
execution_chain&& operator>>(source<T>& src, execution_element_ptr<T>& element)
{
	execution_chain chain;
	return std::move(chain);
}

template <typename T>
execution_chain& operator>>(execution_chain& chain, execution_element_ptr<T>& element)
{
	return chain;
}

template <typename T>
execution_chain&& operator>>(execution_chain& chain, receiver<T>& recv)
{
	return std::move(chain);
}

} // namespace react