#include "router.h"

void Router::GET(const std::string &path, Handler &handler) {
  routes.insert({path, Route(path, handler, HttpMethod::GET)});
}

void Router::POST(const std::string &path, Handler &handler) {
  routes.insert({path, Route(path, handler, HttpMethod::POST)});
}

void Router::PATCH(const std::string &path, Handler &handler) {
  routes.insert({path, Route(path, handler, HttpMethod::PATCH)});
}

void Router::DELETE(const std::string &path, Handler &handler) {
  routes.insert({path, Route(path, handler, HttpMethod::DELETE)});
}

Route::Route(const std::string &path, Handler &handler, HttpMethod http_method) {
  this->handler = std::move(handler);
  this->http_method = http_method;
  this->parse(path);
}

void Route::parse(const std::string &path) {
  this->parse_segments(path.substr(0, path.find("?")));
  this->parse_queries(path.substr(path.find("?") + 1));
}

void Route::parse_segments(const std::string &segments) {
  std::string segments_path = segments.substr(1);
  segments_path += "/";

  size_t start = 0;
  size_t end = segments_path.find("/");

  while (end != std::string::npos) {
    this->segments.push_back(segments_path.substr(start, end));

    start = end + 1;
    end = segments_path.find("/");
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
    end = queris_path.find("&");
  }
}