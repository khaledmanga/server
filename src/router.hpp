#include <functional>
#include <string>
#include <unorderd_map>
#include <vector>

using Handler = std::function<Reponse(Request &)>

    class Route {
private:
  std::vector<std::string> segments;
  std::unorderd_map<std::string, std::string> query;

public:
  Route(const std::string &path);
  std::vector<std::string> parse_segments(const std::string &path);
  std::unorderd_map<std::string, std::string>
  parse_query(const std::string &path);
  void parse_path(const std::string &path);
};

class Router {
private:
  std::unorderd_map<Route, Handler> routes;

  void add(const std::string &path, Handler &handler);
  void match(const std::string& path);
}
