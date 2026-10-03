#include "app.hpp"

#include "../server/server.hpp"

#include <utility>

void App::get(const std::string &path, Handler handler) {
  this->router.get(path, std::move(handler));
}

void App::post(const std::string &path, Handler handler) {
  this->router.post(path, std::move(handler));
}

void App::listen(int port) {
  Server server(port, this->router);
  server.run();
}
