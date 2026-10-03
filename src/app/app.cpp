#include "app.hpp"

#include "../server/server.hpp"

void App::use(Router &router) { this->router = &router; }

void App::use(ThreadPool &threadPool) { this->threadPool = &threadPool; }

void App::use(Logger &logger) { this->logger = &logger; }

void App::listen(int port) {
  Server server(port, *this->router, *this->logger);
  server.run();
}
