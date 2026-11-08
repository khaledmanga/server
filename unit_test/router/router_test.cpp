#include "router/router.hpp"

#include <gtest/gtest.h>

#include <sstream>

TEST(RouteTest, ParsesQueryParametersAndPathSegments) {
  Route route("/echo/abc?a=1&b=2");

  EXPECT_EQ(route.path, "/echo/abc?a=1&b=2");
  ASSERT_EQ(route.segments.size(), 2);
  EXPECT_EQ(route.segments[0], "echo");
  EXPECT_EQ(route.segments[1], "abc");
  EXPECT_EQ(route.params.at("a"), "1");
  EXPECT_EQ(route.params.at("b"), "2");
}

TEST(RouteTest, MatchesStaticPath) {
  Handler handler = [](Request &, Response &) {};
  Route route("/echo/abc?x=1", HTTP_METHOD::GET, handler);

  EXPECT_TRUE(route.match("/echo/abc?x=1", HTTP_METHOD::GET, handler));
}

TEST(RouteTest, RejectsDifferentMethod) {
  Handler handler = [](Request &, Response &) {};
  Route route("/echo/abc?x=1", HTTP_METHOD::GET, handler);

  EXPECT_FALSE(route.match("/echo/abc?x=1", HTTP_METHOD::POST, handler));
}

TEST(RouteTest, RejectsDifferentSegmentCount) {
  Handler handler = [](Request &, Response &) {};
  Route route("/echo/abc?x=1", HTTP_METHOD::GET, handler);

  EXPECT_FALSE(route.match("/echo?x=1", HTTP_METHOD::GET, handler));
}

TEST(RouteTest, RejectsDifferentStaticSegment) {
  Handler handler = [](Request &, Response &) {};
  Route route("/echo/abc?x=1", HTTP_METHOD::GET, handler);

  EXPECT_FALSE(route.match("/echo/xyz?x=1", HTTP_METHOD::GET, handler));
}

TEST(RouteTest, MatchesDynamicSegment) {
  Handler handler = [](Request &, Response &) {};
  Route route("/echo/abc?x=1", HTTP_METHOD::GET, handler);

  EXPECT_TRUE(route.match("/echo/:id?x=1", HTTP_METHOD::GET, handler));
}

TEST(RouteTest, StreamsParametersAndSegments) {
  Route route("/echo/abc?a=1&b=2");
  std::ostringstream output;

  output << route;

  EXPECT_NE(output.str().find("Params:\n"), std::string::npos);
  EXPECT_NE(output.str().find("- a: 1\n"), std::string::npos);
  EXPECT_NE(output.str().find("- b: 2\n"), std::string::npos);
  EXPECT_NE(output.str().find("Segments:\n"), std::string::npos);
  EXPECT_NE(output.str().find("- echo\n"), std::string::npos);
  EXPECT_NE(output.str().find("- abc\n"), std::string::npos);
}

TEST(RouterTest, RegistersEachHttpMethodAndInvokesHandler) {
  Handler handler = [](Request &, Response &) {};
  Router router;

  router.GET("/get?x=1", handler);
  router.POST("/post?x=1", handler);
  router.PUT("/put?x=1", handler);
  router.DELETE("/delete?x=1", handler);

  ASSERT_EQ(router.routes.size(), 4);
  EXPECT_EQ(router.routes.at("/get?x=1")->method, HTTP_METHOD::GET);
  EXPECT_EQ(router.routes.at("/post?x=1")->method, HTTP_METHOD::POST);
  EXPECT_EQ(router.routes.at("/put?x=1")->method, HTTP_METHOD::PUT);
  EXPECT_EQ(router.routes.at("/delete?x=1")->method, HTTP_METHOD::DELETE);

  Request request{};
  Response response(StatusLine("HTTP/1.1", 200, "OK"), Header{}, Body{});
  router.routes.at("/get?x=1")->handler(request, response);
}

TEST(RouterTest, FindsMatchingRoute) {
  Handler handler = [](Request &, Response &) {};
  Router router;
  router.GET("/echo/:id?x=1", handler);
  HTTP_METHOD method = HTTP_METHOD::GET;

  Route *matched = router.match("/echo/:id?x=1", method, handler);

  ASSERT_NE(matched, nullptr);
  EXPECT_EQ(matched->path, "/echo/:id?x=1");
}

TEST(RouterTest, ReturnsNullWhenNoRouteMatches) {
  Handler handler = [](Request &, Response &) {};
  Router router;
  router.GET("/echo/:id?x=1", handler);
  HTTP_METHOD method = HTTP_METHOD::POST;

  EXPECT_EQ(router.match("/echo/:id?x=1", method, handler), nullptr);
}

TEST(RouterTest, ReturnsNullWhenNoRoutesAreRegistered) {
  Handler handler = [](Request &, Response &) {};
  Router router;
  HTTP_METHOD method = HTTP_METHOD::GET;

  EXPECT_EQ(router.match("/missing?x=1", method, handler), nullptr);
}
