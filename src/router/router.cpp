#include "router.hpp"

#include <utility>

namespace {
std::string normalizePath(const std::string &target) {
  const std::size_t query = target.find('?');
  std::string path = target.substr(0, query);

  if (path.empty()) {
    return "/";
  }

  while (path.size() > 1 && path.back() == '/') {
    path.pop_back();
  }

  return path;
}
} // namespace

void Router::add(const std::string &method, const std::string &path,
                 Handler handler) {
  this->routes.push_back({method, normalizePath(path), std::move(handler)});
}

void Router::get(const std::string &path, Handler handler) {
  this->add("GET", path, std::move(handler));
}

void Router::post(const std::string &path, Handler handler) {
  this->add("POST", path, std::move(handler));
}

bool Router::handle(Request &request, Response &response) const {
  const std::string path = normalizePath(request.request_line.target);

  for (const Route &route : this->routes) {
    if (route.method == request.request_line.method && route.path == path) {
      route.handler(request, response);
      return true;
    }
  }

  response.status(HTTP::StatusCode::NOT_FOUND).send("Not Found");
  return false;
}
