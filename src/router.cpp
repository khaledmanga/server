#include "router.hpp"

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

const std::vector<std::string> &Route::getSegments() const { return segments; }

std::vector<std::string> Route::parse_segments(const std::string &path) {

  std::vector<std::string> result;

  const std::size_t query_pos = path.find('?');

  std::string line =
      path.substr(0, query_pos == std::string::npos ? path.size() : query_pos);
      
  if (line == "/") {
    return result;
  }

  if (line.empty() || line.back() != '/') {
    line += '/';
  }

  std::size_t start = 0;

  if (line.front() == '/') {
    start = 1;
  }

  std::size_t end = line.find('/', start);

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

  const std::size_t query_pos = path.find('?');

  if (query_pos == std::string::npos) {
    return result;
  }

  std::string line = path.substr(query_pos + 1);

  std::size_t start = 0;

  while (start < line.size()) {

    std::size_t end = line.find('&', start);

    if (end == std::string::npos) {
      end = line.size();
    }

    std::string query_element = line.substr(start, end - start);

    std::size_t eq_pos = query_element.find('=');

    if (eq_pos != std::string::npos) {

      std::string key = query_element.substr(0, eq_pos);

      std::string value = query_element.substr(eq_pos + 1);

      result[key] = value;
    }

    start = end + 1;
  }

  return result;
}

void Route::parse_path(const std::string &path) {

  segments = parse_segments(path);
  query = parse_query(path);
}

Route::Route(const std::string &path) { parse_path(path); }

void Router::get(const std::string &path, Handler handler) {
  routes.push_back({"GET", Route(path), std::move(handler)});
}

void Router::post(const std::string &path, Handler handler) {
  routes.push_back({"POST", Route(path), std::move(handler)});
}

void Router::add(const std::string &path, Handler handler) {
  get(path, std::move(handler));
}

void Router::use(Middleware middleware) {
  middlewares.emplace_back(std::move(middleware));
}

Response Router::handle(const std::string &path, Request &request) const {
  Response response;
  const auto *matched_route = match(request.method(), path);

  Next next = [&] {
    if (matched_route != nullptr) {
      matched_route->handler(request, response, [] {});
      return;
    }

    StatusLine status_line;
    status_line.setStatusCode(404).setReasonPhrase("Not Found");
    response.setStatusLine(status_line).send("<h1>404 Not Found</h1>");
  };

  for (auto middleware = middlewares.rbegin(); middleware != middlewares.rend();
       ++middleware) {
    Next current = std::move(next);
    next = [middleware, current = std::move(current), &request, &response] {
      (*middleware)(request, response, current);
    };
  }

  next();
  return response;
}

const Router::RouteEntry *Router::match(const std::string &method,
                                        const std::string &path) const {

  std::vector<std::string> request_path = Route::parse_segments(path);

  const RouteEntry *best_match = nullptr;

  std::size_t best_score = 0;

  for (const auto &route : routes) {

    if (route.method != method) {
      continue;
    }

    const auto &route_segments = route.route.getSegments();

    if (request_path.size() != route_segments.size()) {
      continue;
    }

    bool matched = true;
    std::size_t score = 0;

    for (std::size_t i = 0; i < route_segments.size(); ++i) {

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

    if (matched && (best_match == nullptr || score > best_score)) {

      best_match = &route;
      best_score = score;
    }
  }

  return best_match;
}