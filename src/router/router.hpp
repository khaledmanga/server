#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <utility>

#include "../request/request.cpp"
#include "../response/response.cpp"

using Handler = std::function<void(Request *req, Response *res)>

class Route {
  public:
    Router(const std::string& path);
    void parseSegments(std::string& path);
    void parseQueries(std::string& path);
    void parse(const std::string& path);
    std::vector<std::string> segments;
    std::unordered_map<std::string, std::string> querys;
};

class Router {
  Router() = default;
  std::vector<std::pair<Route, Handler>> routes;
  void add(const string& path, Handler handler);
  Route match();
} 
