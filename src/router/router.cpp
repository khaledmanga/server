#include "router.hpp"

Route::Route(const std::string &path, HTTP_METHOD method, Handler handler) : path(path), method(method), handler(handler) { parse(path); }

void Route::parse_params(std::string params_line) {
  params_line += "&";

  size_t start = 0;

  while (start < params_line.size()) {
    size_t end = params_line.find("&", start);
    std::string param_line = params_line.substr(start, end - start);

    size_t equal_pos = param_line.find("=");
    std::string key = param_line.substr(0, equal_pos);
    std::string value = param_line.substr(equal_pos + 1);

    this->params[key] = value;

    start = end + 1;
  }
}

void Route::parse_segments(std::string segments_line) {
  segments_line += "/";

  size_t start = 1;

  while (start < segments_line.size()) {
    size_t end = segments_line.find("/", start);
    std::string segment_line = segments_line.substr(start, end - start);

    this->segments.push_back(segment_line);

    start = end + 1;
  }
}

void Route::parse(const std::string &path) {
  size_t question_pos = path.find("?");
  this->parse_segments(path.substr(0, question_pos));
  this->parse_params(path.substr(question_pos + 1));
}

bool Route::match(const std::string &path, HTTP_METHOD method, Handler handler) {
  Route route(path, method, handler);

  if (route.method != this->method) {
    return false;
  }

  if (route.segments.size() != this->segments.size()) {
    return false;
  }

  for (size_t pos = 0; pos < this->segments.size(); ++pos) {
    if (route.segments[pos][0] == ':') {
      continue;
    }

    if (this->segments[pos] != route.segments[pos]) {
      return false;
    }
  }

  return true;
}

std::ostream &operator<<(std::ostream &os, const Route &route) {
  os << "Params:\n";

  for (auto const &[key, value] : route.params) {
    os << "- " << key << ": " << value << "\n";
  }

  os << "Segments:\n";

  for (auto const &segment : route.segments) {
    os << "- " << segment << "\n";
  }

  return os;
}

Route *Router::match(const std::string &path, HTTP_METHOD &method, Handler &handler) {
  for (auto &route : this->routes) {
    if (route.second->match(path, method, handler)) {
      return route.second.get();
    }
  }

  return nullptr;
}

void Router::GET(const std::string &path, Handler handler) { this->routes[path] = std::make_unique<Route>(path, HTTP_METHOD::GET, handler); }
void Router::POST(const std::string &path, Handler handler) { this->routes[path] = std::make_unique<Route>(path, HTTP_METHOD::POST, handler); }
void Router::PUT(const std::string &path, Handler handler) { this->routes[path] = std::make_unique<Route>(path, HTTP_METHOD::PUT, handler); }
void Router::DELETE(const std::string &path, Handler handler) { this->routes[path] = std::make_unique<Route>(path, HTTP_METHOD::DELETE, handler); }