#include <atomic>
#include <csignal>

#include <iostream>
#include <thread>

#include <unistd.h>

#include "context.hpp"
#include "event_loop.hpp"
#include "logger.hpp"
#include "router.hpp"
#include "server.hpp"
#include "thread_pool.hpp"

int signal_event_fd = -1;

void signal_handler(int) {
  uint64_t value = 1;

  write(signal_event_fd, &value, sizeof(value));
}

int main() {
  try {
    Logger logger;
    logger.addSink(std::make_unique<Terminal>());

    EventLoop event_loop;

    signal_event_fd = event_loop.eventFd();

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    ThreadPool thread_pool(std::thread::hardware_concurrency());
    Router router;

    router.use([&logger](Request &request, Response &response, Next next) {
      logger.log(LogLevel::INFO,
                 request.method() + " " + request.path() + " started");

      next();

      logger.log(

          LogLevel::INFO,
          request.method() + " " + request.path() + " " +
              std::to_string(response.statusLine().statusCode()));
    });

    router.get("/", [](Request &, Response &response, Next) {
      response.send("<h1>Hello World</h1>");
    });

    Server server(8004);

    server.use(logger);
    server.use(thread_pool);
    server.use(event_loop);
    server.use(router);

    server.run();
  } catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
  }

  return 0;
}
