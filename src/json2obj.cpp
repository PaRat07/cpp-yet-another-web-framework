#include "../include/cppdantic/json2obj.h"

std::expected<std::string, ValidationError> operator|(const boost::json::value &from, As<std::string>) noexcept {
    if (from.is_string()) [[likely]] {
        return { { from.get_string().begin(), from.get_string().end() } };
    } else [[unlikely]] {
        return std::unexpected(ValidationError {
            .error = "Field \"{}\" has incorrect type, expected string, got "s + std::string(to_string(from.kind())),
            .field_name = ""s
        });
    }
}