#pragma once

#include "domain.h"

std::optional<std::string> operator|(const boost::json::value &from, As<std::string>) noexcept {
    if (from.is_string()) [[likely]] {
        return { { from.get_string().begin(), from.get_string().end() } };
    } else [[unlikely]] {
        return std::nullopt;
    }
}

template<std::ranges::range ToT>
std::optional<ToT> operator|(const boost::json::value &from, As<ToT>) noexcept {
    if (from.is_array()) [[likely]] {
        return from.get_object();
    } else [[unlikely]] {
        return std::nullopt;
    }
}

template<std::integral ToT>
std::optional<ToT> operator|(const boost::json::value &from, As<ToT>) noexcept {
    if (from.is_int64()) [[likely]] {
        return from.get_int64();
    } else [[unlikely]] {
        return std::nullopt;
    }
}

template<std::floating_point ToT>
std::optional<ToT> operator|(const boost::json::value &from, As<ToT>) noexcept {
    if (from.is_double()) [[likely]] {
        return from.get_double();
    } else [[unlikely]] {
        return std::nullopt;
    }
}

template<size_t Ind>
inline bool ValidateField(auto &ans, const boost::json::object &from) noexcept {
    auto res_json = from.if_contains(boost::pfr::get_name<Ind, std::remove_reference_t<decltype(ans)>>());
    if (res_json == nullptr) [[unlikely]] {
        return true;
    }
    auto res = *res_json | As<std::remove_reference_t<decltype(boost::pfr::get<Ind>(ans))>>();
    if (res.has_value()) [[likely]] {
        boost::pfr::get<Ind>(ans) = std::move(*res);
        return false;
    } else [[unlikely]] {
        return true;
    }
}

template<typename ToT>
inline bool Validate(const boost::json::object &from, ToT &ans) noexcept {
    return ![&from, &ans] <size_t... Inds> (std::index_sequence<Inds...>) noexcept {
        return ([&from, &ans] <size_t Ind> (std::index_sequence<Ind>) noexcept {
            return ValidateField<Ind>(ans, from);
        } (std::index_sequence<Inds>()) || ...);
    } (std::make_index_sequence<boost::pfr::tuple_size<ToT>::value>());
}

template<Class ToT>
inline std::optional<ToT> operator|(const boost::json::value &from, As<ToT>) noexcept {
    if (!from.is_object()) [[unlikely]] {
        return std::nullopt;
    }
    if (ToT ans; Validate(from.get_object(), ans)) [[likely]] {
        return ans;
    } else [[unlikely]] {
        return std::nullopt;
    }
}