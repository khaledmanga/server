#include "app.hpp"

#include "../server/server.hpp"

#include <stdexcept>

void App::use(Router &router) { this->router = &router; }

void App::use(ThreadPool &threadPool) { this->threadPool = &threadPool; }

void App::use(Logger &logger) { this->logger = &logger; }

void App::listen(int port) {
  if (this->router == nullptr || this->logger == nullptr) {
    throw std::logic_error("App requires a Router and Logger before listen()");
  }

  Server server(port, *this->router, *this->logger);
  server.run();
}
