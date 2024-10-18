#pragma once

#include "domain.h"
#include <expected>

using namespace std::literals;

struct ValidationError {
    std::string error;
    std::string field_name; // reversed
};

inline std::expected<std::string_view, ValidationError> operator|(simdjson::ondemand::value &from, As<std::string_view>) noexcept {
    std::string_view res;
    if (const auto ec = from.get_string().get(res); ec == simdjson::SUCCESS) [[likely]] {
        return res;
    } else [[unlikely]] {
        return std::unexpected(ValidationError {
            .error = "Field \"{}\" has incorrect type, expected string"s,
            .field_name = ""s
        });
    }
}

inline std::expected<std::string, ValidationError> operator|(simdjson::ondemand::value &from, As<std::string>) noexcept {
    std::string_view res;
    if (const auto ec = from.get_string().get(res); ec == simdjson::SUCCESS) [[likely]] {
        return std::string(res);
    } else [[unlikely]] {
        return std::unexpected(ValidationError {
            .error = "Field \"{}\" has incorrect type, expected string"s,
            .field_name = ""s
        });
    }
}



template<std::ranges::range ToT> requires (!std::is_same_v<std::string, std::decay_t<ToT>>)
std::expected<ToT, ValidationError> operator|(simdjson::ondemand::value &from, As<ToT>) noexcept {
    ToT ans;
    if (from.get_array().get(ans) == simdjson::SUCCESS) [[likely]] {
        return from.get_object();
    } else [[unlikely]] {
        return std::unexpected(ValidationError {
            .error = "Field \"{}\" has incorrect type, expected array"s,
            .field_name = ""s
        });
    }
}

template<std::integral ToT>
std::expected<ToT, ValidationError> operator|(simdjson::ondemand::value &from, As<ToT>) noexcept {
    int64_t ans = 0;
    if (from.get_int64().get(ans) == simdjson::SUCCESS) [[likely]] {
        return from.get_int64();
    } else [[unlikely]] {
        return std::unexpected(ValidationError {
            .error = "Field \"{}\" has incorrect type, expected integer"s,
            .field_name = ""s
        });
    }
}

template<std::floating_point ToT>
std::expected<ToT, ValidationError> operator|(simdjson::ondemand::value &from, As<ToT>) noexcept {
    double ans;
    if (from.get_double().get(ans) == simdjson::SUCCESS) [[likely]] {
        return from.get_double();
    } else [[unlikely]] {
        return std::unexpected(ValidationError {
            .error = "Field \"{}\" has incorrect type, expected array"s,
            .field_name = ""s
        });
    }
}

template<size_t Ind, typename ToT>
auto ValidateField(simdjson::ondemand::object &from, ToT &res) noexcept -> std::optional<ValidationError> {
    simdjson::ondemand::value res_json;
    auto res_json_unchecked = from.find_field(boost::pfr::get_name<Ind, std::decay_t<ToT>>());
    if (res_json_unchecked.get(res_json) != simdjson::SUCCESS) [[unlikely]] {
        return ValidationError {
            .error = R"(Unable to find field "{}")",
            .field_name = std::string(boost::pfr::get_name<Ind, std::decay_t<ToT>>())
        };
    }
    auto res_opt = res_json | As<std::decay_t<decltype(boost::pfr::get<Ind>(std::decay_t<ToT>()))>>();
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
std::optional<ValidationError> Validate(simdjson::ondemand::object &from, ToT &ans) noexcept {
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

template<Class ToT> requires (!std::is_same_v<ToT, std::string_view>) && (!std::is_same_v<ToT, std::string>)
inline std::expected<ToT, ValidationError> operator|(simdjson::ondemand::value &from, As<ToT>) noexcept {
    auto from_obj = simdjson::ondemand::object();
    if (from.get_object().get(from_obj) != simdjson::SUCCESS) [[unlikely]] {
        return std::unexpected(ValidationError {
            .error = "Field \"{}\" has incorrect type, expected object"s,
            .field_name = ""s
        });
    }
    ToT ans;
    if (auto err = Validate(from_obj, ans); err.has_value()) [[unlikely]] {
        return std::unexpected(*err);
    } else [[likely]] {
        return ans;
    }
}