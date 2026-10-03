#pragma once

#include <functional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "../logger/logger.hpp"
#include "../request/request.hpp"
#include "../response/response.hpp"

using Handler = std::function<void(Request &, Response &, Logger &)>;

class Router {
 public:
  template <typename Callback>
  void get(const std::string &path, Callback &&handler) {
    this->add("GET", path, adaptHandler(std::forward<Callback>(handler)));
  }

  template <typename Callback>
  void post(const std::string &path, Callback &&handler) {
    this->add("POST", path, adaptHandler(std::forward<Callback>(handler)));
  }

  bool handle(Request &request, Response &response,
              Logger *logger = nullptr) const;

 private:
  struct Route {
    std::string method;
    std::string path;
    Handler handler;
  };

  std::vector<Route> routes;
  void add(const std::string &method, const std::string &path, Handler handler);

  template <typename Callback>
  static Handler adaptHandler(Callback &&callback) {
    using Callable = std::decay_t<Callback>;
    if constexpr (std::is_invocable_v<Callable &, Request &, Response &,
                                      Logger &>) {
      return [handler = Callable(std::forward<Callback>(callback))](
                 Request &request, Response &response, Logger &logger) mutable {
        handler(request, response, logger);
      };
    } else {
      static_assert(std::is_invocable_v<Callable &, Request &, Response &>,
                    "Route handler must accept Request and Response, "
                    "optionally followed by Logger");
      return [handler = Callable(std::forward<Callback>(callback))](
                 Request &request, Response &response, Logger &) mutable {
        handler(request, response);
      };
    }
  }
};
