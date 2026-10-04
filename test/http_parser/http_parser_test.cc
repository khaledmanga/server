#include "../../src/http_parser/http_parser.h"

#include <gtest/gtest.h>

#include <string>

TEST(HttpParserTest, LeavesIncompleteRequestLineUnconsumed) {
  Request request;
  HttpRequestState state = HttpRequestState::RequestLine;
  std::string raw_request = "GET /items HTTP/1.1";

  EXPECT_EQ(httpParser(request, state, raw_request), HttpRequestState::RequestLine);
  EXPECT_EQ(state, HttpRequestState::RequestLine);
  EXPECT_EQ(raw_request, "GET /items HTTP/1.1");
  EXPECT_TRUE(request.request_line.method.empty());
}

TEST(HttpParserTest, ParsesGetRequestAndHeaders) {
  Request request;
  HttpRequestState state = HttpRequestState::RequestLine;
  std::string raw_request =
      "GET /items HTTP/1.1\r\n"
      "Host: example.com\r\n"
      "Content-Type: text/plain\r\n"
      "Content-Length: 0\r\n"
      "X-Request-Id: abc123\r\n"
      "\r\n";

  EXPECT_EQ(httpParser(request, state, raw_request), HttpRequestState::Completed);
  EXPECT_EQ(state, HttpRequestState::Completed);
  EXPECT_EQ(request.request_line.method, "GET");
  EXPECT_EQ(request.request_line.target, "/items");
  EXPECT_EQ(request.request_line.protocol, "HTTP");
  EXPECT_EQ(request.request_line.major_version, 1);
  EXPECT_EQ(request.request_line.minor_version, 1);
  EXPECT_EQ(request.header.fields.at("Host"), "example.com");
  EXPECT_EQ(request.header.fields.at("Content-Type"), "text/plain");
  EXPECT_EQ(request.header.fields.at("Content-Length"), "0");
  EXPECT_EQ(request.header.fields.at("X-Request-Id"), "abc123");
  EXPECT_TRUE(request.body.value.empty());
  EXPECT_TRUE(raw_request.empty());
}

TEST(HttpParserTest, CompletesRequestWithoutContentLength) {
  Request request;
  HttpRequestState state = HttpRequestState::RequestLine;
  std::string raw_request =
      "GET /items HTTP/1.1\r\n"
      "Host: example.com\r\n"
      "\r\n";

  EXPECT_EQ(httpParser(request, state, raw_request), HttpRequestState::Completed);
  EXPECT_EQ(state, HttpRequestState::Completed);
  EXPECT_EQ(request.request_line.method, "GET");
  EXPECT_EQ(request.header.fields.at("Host"), "example.com");
  EXPECT_EQ(request.body.value, "");
  EXPECT_TRUE(raw_request.empty());
  EXPECT_EQ(request.header.fields.count("Content-Length"), 0u);
}

TEST(HttpParserTest, ParsesPostBodyAcrossChunks) {
  Request request;
  HttpRequestState state = HttpRequestState::RequestLine;
  std::string raw_request =
      "POST /items HTTP/1.1\r\n"
      "Host: example.com\r\n"
      "Content-Length: 5\r\n"
      "\r\n"
      "hel";

  EXPECT_EQ(httpParser(request, state, raw_request), HttpRequestState::Body);
  EXPECT_EQ(state, HttpRequestState::Body);
  EXPECT_TRUE(request.body.value.empty());
  EXPECT_EQ(raw_request, "hel");

  raw_request += "lo";
  EXPECT_EQ(httpParser(request, state, raw_request), HttpRequestState::Completed);
  EXPECT_EQ(state, HttpRequestState::Completed);
  EXPECT_EQ(request.header.fields.at("Content-Length"), "5");
  EXPECT_EQ(request.body.value, "hello");
  EXPECT_TRUE(raw_request.empty());
}

TEST(HttpParserTest, LeavesUnsupportedMethodUnparsed) {
  Request request;
  HttpRequestState state = HttpRequestState::RequestLine;
  std::string raw_request = "DELETE /items HTTP/1.1\r\nHost: example.com\r\n\r\n";

  EXPECT_EQ(httpParser(request, state, raw_request), HttpRequestState::RequestLine);
  EXPECT_EQ(state, HttpRequestState::RequestLine);
  EXPECT_EQ(raw_request, "DELETE /items HTTP/1.1\r\nHost: example.com\r\n\r\n");
  EXPECT_TRUE(request.request_line.method.empty());
}
