#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../logger/logger.h"
#include "../request/request.h"
#include "../response/response.h"

using Handler = std::function<void(Request &, Response &, Logger &)>;

class Router {
 public:
  void get(const std::string &path, Handler handler) { add("GET", path, std::move(handler)); }

  void post(const std::string &path, Handler handler) { add("POST", path, std::move(handler)); }

  bool handle(Request &request, Response &response, Logger *logger = nullptr) const;

 private:
  struct Route {
    std::string method;
    std::string path;
    Handler handler;
  };

  std::vector<Route> routes;

  void add(const std::string &method, const std::string &path, Handler handler);
};