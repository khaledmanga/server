#ifndef ROUTER_ROUTER_HPP
#define ROUTER_ROUTER_HPP

#include "include/common.h"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Route {
public:
  std::string path;
  std::unordered_map<std::string, std::string> params;
  std::vector<std::string> segments;
  Handler handler;
  HTTP_METHOD method;

  Route(const std::string &path, HTTP_METHOD method = HTTP_METHOD::GET, Handler handler = {});
  void parse_params(std::string params_line);
  void parse_segments(std::string segments_line);
  void parse(const std::string &path);
  bool match(const std::string &path, HTTP_METHOD method = HTTP_METHOD::GET, Handler handler = {});
  friend std::ostream &operator<<(std::ostream &os, const Route &route);
};

class Router {
public:
  std::unordered_map<std::string, std::unique_ptr<Route>> routes;
  void GET(const std::string &path, Handler handler);
  void POST(const std::string &path, Handler handler);
  void PUT(const std::string &path, Handler handler);
  void DELETE(const std::string &path, Handler handler);
  Route *match(const std::string &path, HTTP_METHOD &method, Handler &handler) const;
};

#endif // ROUTER_ROUTER_HPP