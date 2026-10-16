#pragma once

#include "http/request/request.h"
#include "http/response/response.h"
#include "include/common.h"
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

using Handler = std::function<void(Request &, Response &)>;

class Route {
private:
  Handler handler;
  HttpMethod http_method;
  std::vector<std::string> segments;
  std::unordered_map<std::string, std::string> queries;

public:
  Route(const std::string &path, Handler &handler, HttpMethod http_method);
  void parse(const std::string &path);
  void parse_segments(const std::string &segments);
  void parse_queries(const std::string &queries);
};

class Router {
private:
  std::unordered_map<std::string, Route> routes;

public:
  Router() = default;
  void GET(const std::string &path, Handler &handler);
  void POST(const std::string &path, Handler &handler);
  void PATCH(const std::string &path, Handler &handler);
  void DELETE(const std::string &path, Handler &handler);
  Route match(std::string path);
  void handle(Route &route, Request &req, Response &res);
};
