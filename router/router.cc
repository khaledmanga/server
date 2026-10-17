#include "router.h"

void Router::GET(const std::string &path, Handler handler) {
  routes.insert({path, Route(path, handler, HttpMethod::GET)});
}

void Router::POST(const std::string &path, Handler handler) {
  routes.insert({path, Route(path, handler, HttpMethod::POST)});
}

void Router::PATCH(const std::string &path, Handler handler) {
  routes.insert({path, Route(path, handler, HttpMethod::PATCH)});
}

void Router::DELETE(const std::string &path, Handler handler) {
  routes.insert({path, Route(path, handler, HttpMethod::DELETE)});
}

Route::Route(const std::string &path, Handler handler, HttpMethod http_method) {
  this->handler = std::move(handler);
  this->http_method = http_method;
  this->parse(path);
}

void Route::parse(const std::string &path) {
  const size_t query = path.find("?");
  this->parse_segments(path.substr(0, query));
  if (query != std::string::npos) {
    this->parse_queries(path.substr(query + 1));
  }
}

void Route::parse_segments(const std::string &segments) {
  std::string segments_path = segments.substr(1);
  segments_path += "/";

  size_t start = 0;
  size_t end = segments_path.find("/");

  while (end != std::string::npos) {
    this->segments.push_back(segments_path.substr(start, end - start));

    start = end + 1;
    end = segments_path.find("/", start);
  }
}

void Route::parse_queries(const std::string &queries) {
  std::string queris_path = queries;
  queris_path += "&";

  size_t start = 0;
  size_t end = queris_path.find("&");

  while (end != std::string::npos) {
    std::string kv = queris_path.substr(start, end);
    std::string key = kv.substr(0, kv.find("="));
    std::string value = kv.substr(kv.find("=") + 1);

    start = end + 1;
    end = queris_path.find("&", start);
  }
}

Route *Router::match(std::string path) {
  for (auto &[_, route] : this->routes) {
    if (route.match(path)) {
      return &route;
    }
  }
  return nullptr;
}

bool Route::match(std::string path) {
  std::vector<std::string> segments_;
  std::string segments_path = path.substr(1);
  segments_path += "/";

  size_t start = 0;
  size_t end = segments_path.find("/");

  while (end != std::string::npos) {
    segments_.push_back(segments_path.substr(start, end - start));

    start = end + 1;
    end = segments_path.find("/", start);
  }

  if (this->segments.size() != segments_.size()) {
    return false;
  }

  for (size_t i = 0; i < this->segments.size(); ++i) {
    if (this->segments[i][0] == ':') {
      continue;
    }

    if (this->segments[i] != segments_[i]) {
      return false;
    }
  }

  return true;
}

void Route::handle(Request &req, Response &res) { this->handler(req, res); }