#include "./app/app.h"

int main() {
  Router router;
  Logger logger;

  router.get("/", [](Request &, Response &response, Logger &logger) {
    logger.Info("Handling GET /");
    response.send("Hello World!");
  });

  App app;
  app.use(router);
  app.use(logger);
  app.listen(3000);
  return 0;
}
