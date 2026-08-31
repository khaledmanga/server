#include "router.hpp"

std::vector<std::string> Route::getSegments() const { return this->segments; }

std::vector<std::string> Route::parse_segments(const std::string &path) {
  std::vector<std::string> result;

  std::string line = path.substr(0, path.find('?')) + "/";

  size_t start = line.find('/') + 1;
  size_t end = line.find('/', start);

  while (end != std::string::npos) {
    if (end > start) {
      result.push_back(line.substr(start, end - start));
    }
    start = end + 1;
    end = line.find('/', start);
  }

  return result;
}

std::unordered_map<std::string, std::string>
Route::parse_query(const std::string &path) {
  std::unordered_map<std::string, std::string> result;

  std::string line = path.substr(path.find('?')) + "&";

  size_t start = 0;
  size_t end = line.find('&');

  while (end != std::string::npos) {
    std::string query_ele = line.substr(start, end - start);
    size_t eq_pos = query_ele.find('=');

    if (eq_pos != std::string::npos) {
      result[query_ele.substr(0, eq_pos)] = query_ele.substr(eq_pos + 1);
    }

    start = end + 1;
    end = line.find('&', start);
  }

  return result;
}

void Route::parse_path(const std::string &path) {
  this->segments = this->parse_segments(path);
  this->query = this->parse_query(path);
}

Route::Route(const std::string &path) { this->parse_path(path); }

void Router::add(const std::string &path, Handler &handler) {
  Route route(path);

  this->routes(route, handler);
}

Route *Router::match(const std::string &path) {
  std::vector<std::string> request_path = parse_segments(path);

  Route *best_match = nullptr;
  size_t best_score = 0;

  for (auto &route : routes) {
    const auto &route_segments = route.getSegments();

    if (request_path.size() != route_segments.size()) {
      continue;
    }

    bool matched = true;
    size_t score = 0;

    for (size_t i = 0; i < route_segments.size(); ++i) {
      const auto &route_segment = route_segments[i];
      const auto &request_segment = request_path[i];

      if (!route_segment.empty() && route_segment[0] == ':') {
        continue;
      }

      if (route_segment != request_segment) {
        matched = false;
        break;
      }

      ++score;
    }

    if (matched && score > best_score) {
      best_match = &route;
      best_score = score;
    }
  }

  return best_match;
}
