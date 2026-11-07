#include "server/server.hpp"

#include <charconv>
#include <cstdlib>
#include <iostream>
#include <system_error>

int main() {
  int port = 8080;

  Server server(port);
  server.run();
}
