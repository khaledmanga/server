#include "router.h"

#include <unordered_map>
#include <utility>
#include <vector>

namespace {
std::string normalizePath(const std::string &target) {
  std::string path = target.substr(0, target.find('?'));

  while (path.size() > 1 && path.back() == '/') {
    path.pop_back();
  }

  return path.empty() ? "/" : path;
}

std::vector<std::string> splitPath(const std::string &path) {
  std::vector<std::string> segments;
  std::size_t start = 0;

  while (true) {
    const std::size_t separator = path.find('/', start);
    segments.push_back(path.substr(start, separator - start));

    if (separator == std::string::npos) {
      return segments;
    }

    start = separator + 1;
  }
}

bool matchPath(const std::string &pattern, const std::string &path, std::unordered_map<std::string, std::string> &params) {
  const std::vector<std::string> pattern_segments = splitPath(pattern);
  const std::vector<std::string> path_segments = splitPath(path);

  if (pattern_segments.size() != path_segments.size()) {
    return false;
  }

  for (std::size_t i = 0; i < pattern_segments.size(); ++i) {
    const std::string &pattern_segment = pattern_segments[i];
    const std::string &path_segment = path_segments[i];

    if (pattern_segment.size() > 1 && pattern_segment.front() == ':') {
      if (path_segment.empty()) {
        return false;
      }
      params[pattern_segment.substr(1)] = path_segment;
    } else if (pattern_segment != path_segment) {
      return false;
    }
  }

  return true;
}
}  // namespace

void Router::add(const std::string &method, const std::string &path, Handler handler) { routes.push_back({method, normalizePath(path), std::move(handler)}); }

bool Router::handle(Request &request, Response &response, Logger *logger) const {
  match(request, response, logger);
  return true;
}

void Router::match(Request &request, Response &response, Logger *logger) const {
  static Logger fallbackLogger;
  Logger &log = logger ? *logger : fallbackLogger;
  const std::string path = normalizePath(request.request_line.target);
  request.path_params.clear();

  for (const Route &route : routes) {
    std::unordered_map<std::string, std::string> path_params;
    if (route.method == request.request_line.method && matchPath(route.path, path, path_params)) {
      request.path_params = std::move(path_params);
      route.handler(request, response, log);
      return;
    }
  }
  response.status(HTTP::StatusCode::NOT_FOUND).send("Not Found");
}