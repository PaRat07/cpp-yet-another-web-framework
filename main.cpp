#include "include/server/zserver.h"

struct Response {
    int num;
    std::string str;
    std::vector<std::string> ss;
};

struct Request {
    int num;
    std::string stroka;
};

namespace routers {
using zrouter = zserver::APIRouter<"/ping">;

zrouter::GET<"/test">
Ping = [] (Request req) {
    return Response {
        .num = req.num,
        .str = req.stroka,
        .ss = {
            "huy",
            "pizda",
            "scovoroda"
        }
    };
};
} // namespace routers

int main() {
    auto app = zserver::ZServer();
    app.include_router<routers::zrouter>();
    app.listen("0.0.0.0", 1234);
}