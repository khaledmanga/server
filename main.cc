#include "server/server.h"
#include <utility>

int main() {
    Logger logger;
    Terminal terminal;
    logger.add_sink(std::make_unique<Terminal>());
    Server server;
    server.use(std::move(logger));
    server.set_port(8080);
    server.run();
    return 0;
}
