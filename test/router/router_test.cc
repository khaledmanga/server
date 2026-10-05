#include "../../src/router/router.h"

#include <gtest/gtest.h>

TEST(RouterTest, MatchesDynamicPathAndProvidesPathParameters) {
  Router router;

  router.get("/users/:user_id/posts/:post_id", [](Request &request, Response &response, Logger &) {
    response.send(request.path_params.at("user_id") + ":" + request.path_params.at("post_id"));
  });

  Request request;
  request.request_line.method = "GET";
  request.request_line.target = "/users/42/posts/7?include=comments";

  Response response;
  Logger logger;

  router.handle(request, response, logger);

  EXPECT_EQ(request.path_params.at("user_id"), "42");
  EXPECT_EQ(request.path_params.at("post_id"), "7");

  EXPECT_NE(response.serialize().find("\r\n\r\n42:7"), std::string::npos);
}

TEST(RouterTest, MatchesDynamicPathWithTrailingSlash) {
  Router router;

  router.get("/users/:user_id", [](Request &, Response &response, Logger &) { response.send("user"); });

  Request request;
  request.request_line.method = "GET";
  request.request_line.target = "/users/42///";

  Response response;
  Logger logger;

  router.handle(request, response, logger);

  EXPECT_NE(response.serialize().find("\r\n\r\nuser"), std::string::npos);

  EXPECT_EQ(request.path_params.at("user_id"), "42");
}

TEST(RouterTest, DoesNotMatchMissingDynamicSegment) {
  Router router;

  router.get("/users/:user_id", [](Request &, Response &response, Logger &) { response.send("user"); });

  Request request;
  request.request_line.method = "GET";

  Response response;
  Logger logger;

  request.request_line.target = "/users";
  router.handle(request, response, logger);

  EXPECT_NE(response.serialize().find("404 Not Found"), std::string::npos);
}

TEST(RouterTest, DoesNotMatchEmptyDynamicSegment) {
  Router router;

  router.get("/users/:user_id", [](Request &, Response &response, Logger &) { response.send("user"); });

  Request request;
  request.request_line.method = "GET";
  request.request_line.target = "/users/";

  Response response;
  Logger logger;

  router.handle(request, response, logger);

  EXPECT_NE(response.serialize().find("404 Not Found"), std::string::npos);
}

TEST(RouterTest, DoesNotMatchExtraPathSegments) {
  Router router;

  router.get("/users/:user_id", [](Request &, Response &response, Logger &) { response.send("user"); });

  Request request;
  request.request_line.method = "GET";
  request.request_line.target = "/users/42/posts";

  Response response;
  Logger logger;

  router.handle(request, response, logger);

  EXPECT_NE(response.serialize().find("404 Not Found"), std::string::npos);
}

TEST(RouterTest, ClearsPathParametersWhenNoRouteMatches) {
  Router router;

  router.get("/users/:user_id", [](Request &, Response &response, Logger &) { response.send("user"); });

  Request request;
  request.request_line.method = "GET";
  request.request_line.target = "/users/42";

  Response response;
  Logger logger;

  router.handle(request, response, logger);

  ASSERT_EQ(request.path_params.at("user_id"), "42");

  request.request_line.target = "/missing";
  response = Response();

  router.handle(request, response, logger);

  EXPECT_TRUE(request.path_params.empty());

  EXPECT_NE(response.serialize().find("404 Not Found"), std::string::npos);
}
