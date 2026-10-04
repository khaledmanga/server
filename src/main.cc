#include <pthread.h>
#include <signal.h>

#include <system_error>

#include "./app/app.h"
#include "./thread_pool/thread_pool.h"

int main() {
  Router router;
  Logger logger;
  ThreadPool threadPool(4);

  router.get("/", [](Request &request, Response &response, Logger &logger) {
    logger.Info("Handling GET /");
    std::cout << request.body << std::endl;
    response.json(json::parse("{\"message\": \"Hello World!\"}"));
  });

  router.post("/", [](Request &request, Response &response, Logger &logger) {
    logger.Info("Handling POST /");
    std::cout << request.body["message"] << std::endl;
    response.json(json::parse("{\"message\": \"Hello World!\"}"));
  });

  App app;
  app.use(router);
  app.use(threadPool);
  app.use(logger);
  app.listen(3000);
  return 0;
}
