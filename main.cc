#include "router/router.h"
#include "server/server.h"
#include <memory>
#include <utility>

int main() {
  Logger logger;
  logger.add_sink(std::make_unique<Terminal>());
  Router router;
  router.GET("/", [](Request &, Response &res) { res.body.value = "Hello\n"; });
  Server server(std::move(router));
  server.use(std::move(logger));
  server.run();
  return 0;
}
