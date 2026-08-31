#include "router.hpp"

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
