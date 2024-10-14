#include "include/server/zserver.h"


namespace routers {
using zrouter = zserver::APIRouter<"/ping">;

struct Response {
    int num;
    std::string str;
    std::vector<std::string> ss;
};

struct RequestBody {
    int num;
    std::string stroka;
};

zrouter::GET<"/test">
Ping = [] (RequestBody req) {
    return Response {
        .num = req.num,
        .str = req.stroka,
        .ss = {
        }
    };
};
} // namespace routers

int main() {
    auto app = zserver::ZServer();
    app.include_router<routers::zrouter>();
    app.listen("0.0.0.0", 1234);
}