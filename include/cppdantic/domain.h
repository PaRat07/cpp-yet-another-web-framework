#pragma once

#include <concepts>

#include <boost/pfr.hpp>
#include "simdjson.h"

#include "valdatable_types.h"

template<typename T>
concept Class = std::is_class_v<T>;

template<typename U, typename V>
concept IsNot = !std::is_same_v<std::remove_reference_t<U>, std::remove_reference_t<V>>;


template<typename>
struct As{};
