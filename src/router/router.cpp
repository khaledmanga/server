#include "router.hpp"

void Router::add(const string& path, Handler handler) {
  this->route.push(std::make_pair(Route(path), handler));
}

Router::Router(const std::string& path) {
  this->parse(path);
}

void Route::parse(const std::string& path) {
  const auto queryPos = path.find("?");

  if(queryPos == std::string::npos) {
    return;
  }
  
  std::string segments = path.substr(0, queryPos;
  std::string queris = path.substr(queryPos + 1);

 this->parseSegments(segments);
 this->parseQueries(queris);
}

void Route::parseSegments(std::string& path) {
  path += "/";

  auto start = 0;
  auto end = path.find("/", 1);

  while(end != std::string::npos) {
    this->segments.push_back(path.substr(start, end - start));
    start = end + 1;
    end = path.find("/");
  }
}

void Route::parseQueries(std::string& path) {
  path += "&";

  std::size_t start = 0;
  std::size_t end = path.find('&');

  while (end != std::string::npos) {
    const std::string param = path.substr(start, end - start);

    const std::size_t equal = param.find('=');


    if (equal != std::string::npos) {

      const std::string key = param.substr(0, equal);
      const std::string value = param.substr(equal + 1);

      queries.insert({key, value});
    }

    start = end + 1;
    end = path.find('&', start);
  }
}
