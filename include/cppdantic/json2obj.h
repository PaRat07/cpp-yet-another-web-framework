#pragma once

#include "domain.h"


std::string operator|(const boost::json::value &from, As<std::string>) {
    return { from.as_string().begin(), from.as_string().end() };
}

template<std::ranges::range ToT>
ToT operator|(const boost::json::value &from, As<ToT>) {
    return from.as_array();
}

template<std::integral ToT>
ToT operator|(const boost::json::value &from, As<ToT>) {
    return from.as_int64();
}

template<std::floating_point ToT>
ToT operator|(const boost::json::value &from, As<ToT>) {
    return from.as_double();
}

template<typename ToT>
ToT operator|(const boost::json::value &from, As<ToT>) {
    if (from.is_object()) {
        std::clog << "obj" << std::endl;
    } else if (from.is_string()) {
        std::clog << "str" << std::endl;
    } else if (from.is_bool()) {
        std::clog << "bool" << std::endl;
    } else if (from.is_array()) {
        std::clog << "arr" << std::endl;
    } else if (from.is_null()) {
        std::clog << "null" << std::endl;
    }
    ToT ans;
    [&from, &ans] <size_t... Inds> (std::index_sequence<Inds...>) {
        ([&from, &ans] <size_t Ind> (std::index_sequence<Ind>) {
            boost::pfr::get<Ind>(ans) = from.at(boost::pfr::get_name<Ind, ToT>()) | As<std::remove_reference_t<decltype(boost::pfr::get<Ind>(ans))>>();
        } (std::index_sequence<Inds>()), ...);
    } (std::make_index_sequence<boost::pfr::tuple_size<ToT>::value>());
    return ans;
}