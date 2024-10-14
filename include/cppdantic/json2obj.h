#pragma once

#include "domain.h"
#include <expected>

using namespace std::literals;

struct ValidationError {
    std::string error;
    std::string field_name; // reversed
};

std::expected<std::string, ValidationError> operator|(const boost::json::value &from, As<std::string>) noexcept;

template<std::ranges::range ToT>
std::expected<ToT, ValidationError> operator|(const boost::json::value &from, As<ToT>) noexcept {
    if (from.is_array()) [[likely]] {
        return from.get_object();
    } else [[unlikely]] {
        return std::unexpected(ValidationError {
            .error = "Field \"{}\" has incorrect type, expected array, got "s + std::string(to_string(from.kind())),
            .field_name = ""s
        });
    }
}

template<std::integral ToT>
std::expected<ToT, ValidationError> operator|(const boost::json::value &from, As<ToT>) noexcept {
    if (from.is_int64()) [[likely]] {
        return from.get_int64();
    } else [[unlikely]] {
        return std::unexpected(ValidationError {
            .error = "Field \"{}\" has incorrect type, expected integer, got "s + std::string(to_string(from.kind())),
            .field_name = ""s
        });
    }
}

template<std::floating_point ToT>
std::expected<ToT, ValidationError> operator|(const boost::json::value &from, As<ToT>) noexcept {
    if (from.is_double()) [[likely]] {
        return from.get_double();
    } else [[unlikely]] {
        return std::unexpected(ValidationError {
            .error = "Field \"{}\" has incorrect type, expected array, got "s + std::string(to_string(from.kind())),
            .field_name = ""s
        });
    }
}

template<size_t Ind, typename ToT>
inline auto ValidateField(const boost::json::object &from, ToT &res) noexcept -> std::optional<ValidationError> {
    auto res_json = from.if_contains(boost::pfr::get_name<Ind, std::decay_t<ToT>>());
    if (res_json == nullptr) [[unlikely]] {
        return ValidationError {
            .error = R"(Unable to find field "{}")",
            .field_name = std::string(boost::pfr::get_name<Ind, std::decay_t<ToT>>())
        };
    }
    auto res_opt = *res_json | As<std::decay_t<decltype(boost::pfr::get<Ind>(std::decay_t<ToT>()))>>();
    if (res_opt.has_value()) [[likely]] {
        boost::pfr::get<Ind>(res) = std::move(*res_opt);
        return std::nullopt;
    } else [[unlikely]] {
        std::string err(boost::pfr::get_name<Ind, std::decay_t<ToT>>());
        if (!res_opt.error().field_name.empty()) {
            err += "::"s + res_opt.error().field_name;
        }
        res_opt.error().field_name = std::move(err);
        return res_opt.error();
    }
}

template<typename ToT, size_t Ind = 0>
inline std::optional<ValidationError> Validate(const boost::json::object &from, ToT &ans) noexcept {
    if constexpr (Ind < boost::pfr::tuple_size_v<ToT>) {
        if (auto res = ValidateField<Ind, ToT>(from, ans); res.has_value()) [[likely]] {
            return *res;
        } else {
            return Validate<ToT, Ind + 1>(from, ans);
        }
    } else {
        return std::nullopt;
    }
}

template<Class ToT>
inline std::expected<ToT, ValidationError> operator|(const boost::json::value &from, As<ToT>) noexcept {
    if (!from.is_object()) [[unlikely]] {
        return std::unexpected(ValidationError {
            .error = "Field \"{}\" has incorrect type, expected object, got "s + std::string(to_string(from.kind())),
            .field_name = ""s
        });
    }
    ToT ans;
    if (auto err = Validate(from.get_object(), ans); err.has_value()) [[unlikely]] {
        return std::unexpected(*err);
    } else [[likely]] {
        return ans;
    }
}