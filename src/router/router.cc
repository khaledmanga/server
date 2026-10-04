#include "router.h"

#include <utility>

namespace {
std::string normalizePath(const std::string &target) {
  std::string path = target.substr(0, target.find('?'));

  while (path.size() > 1 && path.back() == '/') {
    path.pop_back();
  }

  return path.empty() ? "/" : path;
}
}  // namespace

void Router::add(const std::string &method, const std::string &path, Handler handler) { routes.push_back({method, normalizePath(path), std::move(handler)}); }

bool Router::handle(Request &request, Response &response, Logger *logger) const {
  static Logger fallbackLogger;
  Logger &log = logger ? *logger : fallbackLogger;
  const std::string path = normalizePath(request.request_line.target);

  for (const Route &route : routes) {
    if (route.method == request.request_line.method && route.path == path) {
      route.handler(request, response, log);
      return true;
    }
  }

  response.status(HTTP::StatusCode::NOT_FOUND).send("Not Found");
  return false;
}