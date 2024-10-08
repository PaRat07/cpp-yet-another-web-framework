#include <bits/stdc++.h>
#include "zserver.h"


using zrouter = zserver::APIRouter<"/ping">;


zrouter::GET<"">
Ping = [] (const httplib::Request &req, httplib::Response &resp) {
    resp.set_content("holocost", "text/plain");
};


int main() {
    auto app = zserver::ZServer();
    app.include_router<zrouter>();
    app.listen("0.0.0.0", 1234);
}