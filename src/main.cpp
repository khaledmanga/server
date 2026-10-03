#include "./app/app.hpp"

int main() {
  App app;

  app.get("/",
          [](Request &, Response &response) { response.send("Hello World!"); });

  app.listen(3000);
  return 0;
}
