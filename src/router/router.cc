#include "router.h"

Route::Route(const std::string &method, const std::string &path, const Handler &handler) {
  this->method = method;
  this->path = path;
  this->handler = handler;
  this->parse();
}

void Router::add(const std::string &method, const std::string &path, Handler handler) { routes.emplace_back(method, path, std::move(handler)); }

void Router::handle(Request &request, Response &response, Logger &logger) const {
  std::optional<Route> route = this->match(request);

  if (!route) {
    response.status(HTTP::StatusCode::NOT_FOUND);
    response.send("Not Found");
    return;
  }

  route->handler(request, response, logger);
}

std::optional<Route> Router::match(Request &request) const {
  Route request_route(VALUE_EMPTY, request.request_line.target, [](Request &, Response &, Logger &) {});

  for (const auto &route : routes) {
    if (route.match(request_route)) {
      return route;
    }
  }

  return std::nullopt;
}

bool Route::match(const Route &route) const {
  if (route.segments.size() != this->segments.size() || route.queries.size() != this->queries.size()) {
    return false;
  }

  for (size_t i = 0; i < this->segments.size(); ++i) {
    if (!this->segments[i].empty() && this->segments[i][0] == ':') {
      continue;
    }

    if (this->segments[i] != route.segments[i]) {
      return false;
    }
  }

  return true;
}

void Route::parse() {
  this->parseSegments();
  this->parseQueries();
}

void Route::parseSegments() {
  std::string segmentsPart = this->path.substr(1, this->path.find("?"));

  segmentsPart += "/";

  size_t start = 0;
  size_t end = segmentsPart.find("/");

  while (end != std::string::npos) {
    this->segments.push_back(segmentsPart.substr(start, end - start));

    start = end + 1;
    end = segmentsPart.find("/", start);
  }
}

void Route::parseQueries() {
  size_t query_pos = this->path.find("?");

  if (query_pos == std::string::npos) {
    return;
  }

  std::string queriesPart = this->path.substr(query_pos + 1);
  queriesPart += "&";

  size_t start = 0;
  size_t end = queriesPart.find("&");

  while (end != std::string::npos) {
    std::string kv = queriesPart.substr(start, end - start);

    size_t equal_pos = kv.find("=");

    if (equal_pos != std::string::npos) {
      std::string key = kv.substr(0, equal_pos);
      std::string value = kv.substr(equal_pos + 1);

      this->queries[key] = value;
    }

    start = end + 1;
    end = queriesPart.find("&", start);
  }
}

void Router::get(const std::string &path, Handler handler) { add("GET", path, std::move(handler)); }

void Router::post(const std::string &path, Handler handler) { add("POST", path, std::move(handler)); }