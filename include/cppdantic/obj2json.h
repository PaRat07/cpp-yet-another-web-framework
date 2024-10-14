#pragma once

#include "domain.h"

auto operator|(std::integral auto from, As<boost::json::object>) {
    return from;
}

auto operator|(std::floating_point auto from, As<boost::json::object>) {
    return from;
}

boost::json::string_view operator|(std::string_view from, As<boost::json::object>);

auto operator|(std::ranges::range auto from, As<boost::json::object>) requires IsNot<decltype(from), std::string> {
    auto ans = boost::json::array(from.size());
    std::transform(from.begin(), from.end(), ans.begin(), [] (auto x) {
        return x | As<boost::json::object>();
    });
    return ans;
}

template<typename FromT> requires Class<FromT> && IsNot<FromT, std::string> && (!std::ranges::range<FromT>)
boost::json::object operator|(const FromT &from, As<boost::json::object>) {
    boost::json::object resp;
    [&from, &resp] <size_t... Inds> (std::index_sequence<Inds...>) {
        ([&from, &resp] <size_t Ind> (std::index_sequence<Ind>) {
            resp[boost::pfr::get_name<Ind, FromT>()] = boost::pfr::get<Ind>(from) | As<boost::json::object>();
        } (std::index_sequence<Inds>()), ...);
    } (std::make_index_sequence<boost::pfr::tuple_size<FromT>::value>());
    return resp;
}
