#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../logger/logger.h"
#include "../response/response.h"

class Request;

using Handler = std::function<void(Request &, Response &, Logger &)>;

class Route {
 public:
  Route(const std::string &method, const std::string &path, const Handler &handler);
  bool match(const Route &route) const;

  std::string method;
  std::string path;
  Handler handler;

  std::vector<std::string> segments;
  std::unordered_map<std::string, std::string> queries;
  mutable std::unordered_map<std::string, std::string> params;

 private:
  void parse();
  void parseSegments();
  void parseQueries();
};

class Router {
 public:
  void get(const std::string &path, Handler handler);
  void post(const std::string &path, Handler handler);
  void add(const std::string &method, const std::string &path, Handler handler);
  void handle(Request &request, Response &response, Logger &logger) const;

 private:
  std::shared_ptr<Route> match(Request &request) const;
  std::vector<Route> routes;
};

#include "../request/request.h"