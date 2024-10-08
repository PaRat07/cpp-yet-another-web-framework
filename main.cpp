#include "include/server/zserver.h"

struct Response {
    int num;
    std::string str;
};

namespace routers {
using zrouter = zserver::APIRouter<"/ping">;

zrouter::GET<"/test">
Ping = [](const httplib::Request &req) {
    return Response {
        .num = 52,
        .str = "pizda"
    };
};
} // namespace routers

int main() {
    auto app = zserver::ZServer();
    app.include_router<routers::zrouter>();
    app.listen("0.0.0.0", 1234);
}