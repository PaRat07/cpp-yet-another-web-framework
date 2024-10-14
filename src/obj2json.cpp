#include "../include/cppdantic/obj2json.h"

boost::json::string_view operator|(std::string_view from, As<boost::json::object>) {
    return from;
}
