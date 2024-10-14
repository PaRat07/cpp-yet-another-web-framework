#include "../include/server/zserver.h"

#include "httplib.h"

void zserver::ZServer::listen(std::string ip, size_t port) const {
    httplib::Server svr;
    for (const auto &[prefix, handler] : handlers_) {
        svr.Post(prefix, [&handler] (const httplib::Request &req, httplib::Response &resp) {
            HttpRequest zreq = {
                .body = std::move(req.body)
            };
            HttpResponse zresp = {
                .status = resp.status,
                .body = std::move(resp.body)
            };
            handler(zreq, zresp);
            resp.body = std::move(zresp.body);
            resp.status = resp.status;
        });
    }
    svr.set_exception_handler([](const auto& req, auto& res, std::exception_ptr ep) {
        try {
            std::rethrow_exception(ep);
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    });
    svr.set_post_routing_handler([](const auto& req, auto& res) {
        std::cerr << "Got request" << std::endl;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Headers", "*");
    });
    svr.listen(ip, port);
}
