#pragma once
#include <vector>
#include <string_view>
#include <map>

#include "httplib.h"

namespace zserver {
template<size_t N>
struct StringLiteral {
    constexpr StringLiteral(const char (&str)[N]) {
        std::copy_n(str, N, data);
    }
    char data[N];
};

template<StringLiteral prefix, auto = [] {}>
class APIRouter {
 public:
    static constexpr std::string kPrefix = prefix.data;
    template<StringLiteral handler_prefix>
    struct GET {
        static constexpr std::string kHandlerPrefix = handler_prefix.data;
        GET(const auto &handler) {
            APIRouter::handlers_[kHandlerPrefix] = +handler;
        }
    };

    template<typename RouterT>
    void include_router() {
        for (const auto &[pref, handler] : RouterT::handlers_) {
            handlers_[RouterT::kPrefix + pref] = handler;
        }
    }

    template<StringLiteral, auto>
    friend class APIRouter;

 protected:
    // relatively to the kPrefix
    static std::map<std::string, void(*)(const httplib::Request&, httplib::Response&)> handlers_;
};

template<StringLiteral prefix, auto lamb>
std::map<std::string, void(*)(const httplib::Request&, httplib::Response&)> zserver::APIRouter<prefix, lamb>::handlers_ = {};


class ZServer : public APIRouter<""> {
 public:
    void listen(std::string ip, size_t port) const {
        httplib::Server svr;
        for (const auto &[prefix, handler] : handlers_) {
            svr.Get(prefix, *handler);
        }
        svr.set_post_routing_handler([](const auto& req, auto& res) {
            std::cerr << "Got request" << std::endl;
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_header("Access-Control-Allow-Headers", "*");
        });
        svr.listen(ip, port);
    }
};
} // namespace zserver