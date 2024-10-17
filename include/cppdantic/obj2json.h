#pragma once

#include "domain.h"

simdjson::ondemand::value operator|(std::integral auto from, As<simdjson::ondemand::value>) {
    return from;
}

simdjson::ondemand::value operator|(std::floating_point auto from, As<simdjson::ondemand::value>) {
    return from;
}

std::string_view operator|(std::string_view from, As<simdjson::ondemand::value>);

simdjson::ondemand::value operator|(std::ranges::range auto from, As<simdjson::ondemand::value>) requires IsNot<decltype(from), std::string> {
    return { from.begin(), from.end() };
}

template<typename FromT> requires Class<FromT> && IsNot<FromT, std::string> && (!std::ranges::range<FromT>)
simdjson::ondemand::object operator|(const FromT &from, As<simdjson::ondemand::value>) {
    simdjson::ondemand::object resp;
    [&from, &resp] <size_t... Inds> (std::index_sequence<Inds...>) {
        ([&from, &resp] <size_t Ind> (std::index_sequence<Ind>) {
            resp[boost::pfr::get_name<Ind, FromT>()] = boost::pfr::get<Ind>(from) | As<simdjson::ondemand::value>();
        } (std::index_sequence<Inds>()), ...);
    } (std::make_index_sequence<boost::pfr::tuple_size<FromT>::value>());
    return resp;
}
