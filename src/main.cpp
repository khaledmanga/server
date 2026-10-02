#include "./server/server.hpp"

int main() {
  Server app = Server(3000);

  app.run();
  return 0;
}
