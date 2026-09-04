#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "request.hpp"
#include "response.hpp"

using Next = std::function<void()>;
using Handler = std::function<void(Request &, Response &, Next)>;
using Middleware = std::function<void(Request &, Response &, Next)>;

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
  struct RouteEntry {
    std::string method;
    Route route;
    Handler handler;
  };

  std::vector<RouteEntry> routes;
  std::vector<Middleware> middlewares;

public:
  void get(const std::string &path, Handler handler);
  void post(const std::string &path, Handler handler);
  void add(const std::string &path, Handler handler);
  void use(Middleware middleware);
  Response handle(const std::string &path, Request &request) const;
  const RouteEntry *match(const std::string &method,
                          const std::string &path) const;
};
