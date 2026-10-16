#include "server/server.h"
#include "router/router.h"
#include <utility>

int main() {
    Logger logger;
    Terminal terminal;
    logger.add_sink(std::make_unique<Terminal>());
    Router router;
    router.GET("/", [this](Request &req, Response &res) {
        std::cout << "Hello\n";
    })
    Server server;
    server.use(std::move(logger));
    server.set_port(8080);
    server.run();
    return 0;
}
