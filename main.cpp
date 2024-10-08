#include "zserver.h"

namespace routers {
using zrouter = zserver::APIRouter<"/ping">;

zrouter::GET<"/test">
Ping = [](const httplib::Request &req, httplib::Response &resp) {
    resp.set_content(R"({ "holocost" : "yes" })", "application/json");
};
} // namespace routers

int main() {
    auto app = zserver::ZServer();
    app.include_router<routers::zrouter>();
    app.listen("0.0.0.0", 1234);
}