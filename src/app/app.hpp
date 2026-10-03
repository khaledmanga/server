#pragma once

#include "../router/router.hpp"

class App {
public:
  void get(const std::string &path, Handler handler);
  void post(const std::string &path, Handler handler);
  void listen(int port);

private:
  Router router;
};
