#pragma once
#include <vector>
#include <string_view>
#include <map>
#include <format>


#include "../cppdantic/json2obj.h"
#include "../cppdantic/obj2json.h"

namespace zserver {
template<size_t N>
struct StringLiteral {
    constexpr StringLiteral(const char (&str)[N]) {
        std::copy_n(str, N, data);
    }
    char data[N];
};

struct HttpRequest {
    std::string body;
};

struct HttpResponse {
    int status = 200;
    std::string body;
};

static simdjson::ondemand::parser parser;
template<StringLiteral prefix, auto = [] {}>
class APIRouter {
 public:
    static constexpr std::string kPrefix = prefix.data;
    template<StringLiteral handler_prefix>
    struct GET {
        static constexpr std::string kHandlerPrefix = handler_prefix.data;

        GET(const auto &handler) {
            using InvokeT = decltype([] <typename InvT, typename RetT> (RetT(*)(InvT)) {
                return InvT();
            } (+handler));
            APIRouter::handlers_[kHandlerPrefix] = [&handler] (HttpRequest &req, HttpResponse &resp) {
                req.body.reserve(req.body.size() + simdjson::SIMDJSON_PADDING);
                simdjson::ondemand::document doc;
                if (parser.iterate(req.body).get(doc) != simdjson::SUCCESS) [[unlikely]] {
                    resp.status = 400;
                    resp.body = R"("{ "Error" : "Json is incorrect" }")";
                    return;
                }
                auto req_json = simdjson::ondemand::value();
                if (doc.get_value().get(req_json) != simdjson::SUCCESS) {
                    resp.status = 400;
                    resp.body = R"("{ "Error" : "Json is incorrect" }")";
                    return;
                }
                auto request_obj = req_json | As<InvokeT>();
                if (request_obj.has_value()) [[likely]] {
                    resp.body = boost::json::serialize(handler(*request_obj) | As<boost::json::object>());
                } else [[unlikely]] {
                    resp.status = 400;
                    resp.body = std::format(std::runtime_format(request_obj.error().error), request_obj.error().field_name);
                }
            };
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
    static std::map<std::string, std::function<void(HttpRequest&, HttpResponse&)>> handlers_;
};

template<StringLiteral prefix, auto lamb>
std::map<std::string, std::function<void(HttpRequest&, HttpResponse&)>> zserver::APIRouter<prefix, lamb>::handlers_ = {};


class ZServer : public APIRouter<""> {
 public:
    void listen(std::string ip, size_t port) const;
};
} // namespace zserver