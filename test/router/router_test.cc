#include "../../src/router/router.h"

#include <gtest/gtest.h>

TEST(RouterTest, MatchesDynamicPathAndProvidesPathParameters) {
  Router router;

  router.get("/users/:user_id/posts/:post_id", [](Request &request, Response &response, Logger &) {
    ASSERT_NE(request.route, nullptr);
    std::string user_id = request.route->params["user_id"];
    std::string post_id = request.route->params["post_id"];
    response.send(user_id + ":" + post_id);
  });

  Request request;
  request.request_line.method = "GET";
  request.request_line.target = "/users/42/posts/7";

  Response response;
  Logger logger;

  router.handle(request, response, logger);

  ASSERT_NE(request.route, nullptr);
  EXPECT_EQ(request.route->params["user_id"], "42");
  EXPECT_EQ(request.route->params["post_id"], "7");
  EXPECT_NE(response.serialize().find("42:7"), std::string::npos);
}

TEST(RouterTest, MatchesDynamicPathWithTrailingSlash) {
  Router router;

  router.get("/users/:user_id///", [](Request &, Response &response, Logger &) { response.send("user"); });

  Request request;
  request.request_line.method = "GET";
  request.request_line.target = "/users/42///";

  Response response;
  Logger logger;

  router.handle(request, response, logger);

  EXPECT_NE(response.serialize().find("user"), std::string::npos);
  ASSERT_NE(request.route, nullptr);
  EXPECT_EQ(request.route->params["user_id"], "42");
}

TEST(RouterTest, DoesNotMatchMissingDynamicSegment) {
  Router router;

  router.get("/users/:user_id", [](Request &, Response &response, Logger &) { response.send("user"); });

  Request request;
  request.request_line.method = "GET";
  request.request_line.target = "/users";

  Response response;
  Logger logger;

  router.handle(request, response, logger);

  EXPECT_NE(response.serialize().find("Not Found"), std::string::npos);
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

  EXPECT_NE(response.serialize().find("Not Found"), std::string::npos);
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

  EXPECT_NE(response.serialize().find("Not Found"), std::string::npos);
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

  ASSERT_NE(request.route, nullptr);
  ASSERT_EQ(request.route->params["user_id"], "42");

  request.request_line.target = "/missing";
  response = Response();

  router.handle(request, response, logger);

  EXPECT_EQ(request.route, nullptr);
  EXPECT_NE(response.serialize().find("Not Found"), std::string::npos);
}
