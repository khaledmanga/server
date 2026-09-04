#include <gtest/gtest.h>

#include <string>

#include "../src/router.hpp"

TEST(RouterTest, GetHandlerUsesExpressStyleRequestResponseAndNext) {
  Router app;
  std::string execution;

  app.use([&execution](Request &, Response &, Next next) {
    execution += "before,";
    next();
    execution += "after,";
  });
  app.get("/hello", [&execution](Request &request, Response &response, Next) {
    execution += request.method() + ",";
    response.status(201).send("hello");
  });

  Request request;
  request.setMethod("GET").setPath("/hello");
  Response response = app.handle(request.path(), request);

  EXPECT_EQ(execution, "before,GET,after,");
  EXPECT_EQ(response.statusLine().statusCode(), 201);
  EXPECT_EQ(response.body(), "hello");
  EXPECT_EQ(response.headers().contentLength(), 5);
}

TEST(RouterTest, RouteMethodMustMatchRequestMethod) {
  Router app;
  app.get("/hello", [](Request &, Response &response, Next) {
    response.send("get");
  });

  Request request;
  request.setMethod("POST").setPath("/hello");
  Response response = app.handle(request.path(), request);

  EXPECT_EQ(response.statusLine().statusCode(), 404);
  EXPECT_EQ(response.body(), "<h1>404 Not Found</h1>");
}