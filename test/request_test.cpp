#include <gtest/gtest.h>

#include <string>

#include "../src/request.hpp"

TEST(RequestTest, ParseRequestLine) {
  Request request;

  std::string raw = "GET /echo HTTP/1.1";

  request.parseRequestLine(raw);

  EXPECT_EQ(request.requestLine().method(), "GET");
  EXPECT_EQ(request.requestLine().path(), "/echo");
  EXPECT_EQ(request.requestLine().majorVersion(), 1);
  EXPECT_EQ(request.requestLine().minorVersion(), 1);
}

TEST(RequestTest, ParseHeader) {
  Request request;

  std::string raw = "Host: localhost:8080\r\n"
                    "User-Agent: Mozilla/5.0\r\n"
                    "Accept: */*\r\n"
                    "Content-Type: application/json\r\n"
                    "Content-Length: 0\r\n"
                    "\r\n";

  request.parseHeader(raw);

  EXPECT_EQ(request.header().host(), "localhost:8080");
  EXPECT_EQ(request.header().userAgent(), "Mozilla/5.0");
  EXPECT_EQ(request.header().accept(), "*/*");
  EXPECT_EQ(request.header().contentType(), "application/json");
  EXPECT_EQ(request.header().contentLength(), 0);
}

TEST(RequestTest, ParseRequest) {
  Request request;
  RequestState state = RequestState::RequestLine;

  std::string raw = "POST /echo HTTP/1.1\r\n"
                    "Host: localhost:8080\r\n"
                    "User-Agent: Mozilla/5.0\r\n"
                    "Accept: */*\r\n"
                    "Content-Type: text/plain\r\n"
                    "Content-Length: 5\r\n"
                    "\r\n"
                    "Hello";

  bool result = request.parse(raw, state);

  EXPECT_TRUE(result);
  EXPECT_EQ(state, RequestState::Complete);

  EXPECT_EQ(request.requestLine().method(), "POST");
  EXPECT_EQ(request.requestLine().path(), "/echo");
  EXPECT_EQ(request.requestLine().majorVersion(), 1);
  EXPECT_EQ(request.requestLine().minorVersion(), 1);

  EXPECT_EQ(request.header().host(), "localhost:8080");
  EXPECT_EQ(request.header().userAgent(), "Mozilla/5.0");
  EXPECT_EQ(request.header().accept(), "*/*");
  EXPECT_EQ(request.header().contentType(), "text/plain");
  EXPECT_EQ(request.header().contentLength(), 5);

  EXPECT_EQ(request.body(), "Hello");
}
