#include "server/server.h"

int main() {
    Server server;
    server.set_port(8080);
    server.run();
    return 0;
}
