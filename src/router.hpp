#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "request.hpp"
#include "response.hpp"

using Handler = std::function<Response(Request &)>;

class Route {
private:
  std::vector<std::string> segments;
  std::unordered_map<std::string, std::string> query;

public:
  explicit Route(const std::string &path);

  static std::vector<std::string> parse_segments(const std::string &path);
  std::unordered_map<std::string, std::string>
  parse_query(const std::string &path);

  void parse_path(const std::string &path);

  const std::vector<std::string> &getSegments() const;
};

class Router {
private:
  std::vector<std::pair<Route, Handler>> routes;

public:
  void add(const std::string &path, Handler handler);

  const std::pair<Route, Handler> *match(const std::string &path) const;
};