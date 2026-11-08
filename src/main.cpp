#include "server/server.hpp"

#include <charconv>
#include <cstdlib>
#include <iostream>
#include <system_error>

int main() {
  int port = 8080;

  Router router;
  router.GET("/", [](Request &, Response &response) {
    response.header.content_type = "text/html; charset=utf-8";
    response.body.content = "<h1>Hello</h1>";
  });

  Server server(port);
  server.use(router);
  server.logger.add_sink(std::make_unique<Terminal>());
  server.run();
}
