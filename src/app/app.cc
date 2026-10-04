#include "app.h"

#include <stdexcept>

#include "../server/server.h"
#include "../thread_pool/thread_pool.h"

void App::use(Router &router) { this->router = &router; }

void App::use(ThreadPool &threadPool) { this->threadPool = &threadPool; }

void App::use(Logger &logger) { this->logger = &logger; }

void App::listen(int port) {
  if (this->router == nullptr || this->threadPool == nullptr || this->logger == nullptr) {
    throw std::logic_error("Router, thread pool, and logger must be configured before listening");
  }
  Server server(port, *this->router, *this->logger, *this->threadPool);
  server.run();
}
