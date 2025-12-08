#pragma once
#include <type_traits>

template <typename T>
constexpr std::remove_reference_t<T>&& move(T&& t) noexcept;

#include "stdmove.tpp"