#include <pthread.h>
#include <signal.h>

#include <system_error>

#include "./app/app.h"
#include "./thread_pool/thread_pool.h"

int main() {
  sigset_t signals;
  sigemptyset(&signals);
  sigaddset(&signals, SIGINT);
  sigaddset(&signals, SIGTERM);
  const int mask_error = pthread_sigmask(SIG_BLOCK, &signals, nullptr);
  if (mask_error != 0) {
    throw std::system_error(mask_error, std::generic_category(),
                            "pthread_sigmask");
  }

  Router router;
  Logger logger;
  ThreadPool threadPool(4);

  router.get("/", [](Request &, Response &response, Logger &logger) {
    logger.Info("Handling GET /");
    response.send("Hello World!");
  });

  App app;
  app.use(router);
  app.use(threadPool);
  app.use(logger);
  app.listen(3000);
  return 0;
}
