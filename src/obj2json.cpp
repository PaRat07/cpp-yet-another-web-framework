#include "../include/cppdantic/obj2json.h"

std::string_view operator|(std::string_view from, As<simdjson::ondemand::value>) {
    return from;
}
