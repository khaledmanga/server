#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../request/request.hpp"
#include "../response/response.hpp"

using Handler = std::function<void(Request &, Response &)>;

class Router {
public:
  void get(const std::string &path, Handler handler);
  void post(const std::string &path, Handler handler);
  bool handle(Request &request, Response &response) const;

private:
  struct Route {
    std::string method;
    std::string path;
    Handler handler;
  };

  std::vector<Route> routes;
  void add(const std::string &method, const std::string &path, Handler handler);
};
