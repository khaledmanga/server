#pragma once

#include "../router/router.h"

class ThreadPool;

class App {
 public:
  void use(Router &router);
  void use(ThreadPool &threadPool);
  void use(Logger &logger);
  void listen(int port);

 private:
  const Router *router = nullptr;
  ThreadPool *threadPool = nullptr;
  Logger *logger = nullptr;
};
