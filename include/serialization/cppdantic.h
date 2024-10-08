#pragma once

#include <concepts>

#include <boost/pfr.hpp>
#include <boost/json.hpp>

#include "httplib.h"

template<typename T>
concept Class = std::is_class_v<T>;

template<typename U, typename V>
concept IsNot = !std::is_same_v<std::remove_reference_t<U>, std::remove_reference_t<V>>;


template<typename>
struct As{};


boost::json::value operator|(std::integral auto from, As<boost::json::object>) {
    return from;
}

boost::json::value operator|(std::floating_point auto from, As<boost::json::object>) {
    return from;
}

boost::json::string_view operator|(std::string_view from, As<boost::json::object>) {
    return from;
}

boost::json::array operator|(std::ranges::range auto from, As<boost::json::object>) requires IsNot<decltype(from), std::string> {
    return from;
}
template<typename FromT> requires Class<FromT> && IsNot<FromT, std::string>
boost::json::object operator|(const FromT &from, As<boost::json::object>) {
    boost::json::object resp;
    [&from, &resp] <size_t... Inds> (std::index_sequence<Inds...>) {
        ([&from, &resp] <size_t Ind> (std::index_sequence<Ind>) {
            resp[boost::pfr::get_name<Ind, FromT>()] = boost::pfr::get<Ind>(from) | As<boost::json::object>();
        } (std::index_sequence<Inds>()), ...);
    } (std::make_index_sequence<boost::pfr::tuple_size<FromT>::value>());
    return resp;
}
