#include "../../src/request/request.h"

#include <gtest/gtest.h>

#include <sstream>
#include <string>

TEST(RequestTest, StreamsRequestLineFields) {
  RequestLine request_line;
  request_line.method = "GET";
  request_line.target = "/items";
  request_line.protocol = "HTTP";
  request_line.major_version = 1;
  request_line.minor_version = 1;

  std::ostringstream output;
  output << request_line;

  EXPECT_EQ(output.str(),
            "Method: GET\n"
            "Target: /items\n"
            "Protocol: HTTP/1.1");
}

TEST(RequestTest, StreamsHeaderFields) {
  Header header;
  header.host = "example.com";
  header.content_type = "text/plain";
  header.content_length = 5;

  std::ostringstream output;
  output << header;

  EXPECT_EQ(output.str(),
            "Host: example.com\n"
            "Content-Type: text/plain\n"
            "Content-Length: 5");
}

TEST(RequestTest, StreamsBodyValue) {
  Body body;
  body.value = "hello";

  std::ostringstream output;
  output << body;

  EXPECT_EQ(output.str(), "Body: hello");
}

TEST(RequestTest, StreamsCompleteRequest) {
  Request request;
  request.request_line.method = "POST";
  request.request_line.target = "/items";
  request.request_line.protocol = "HTTP";
  request.request_line.major_version = 1;
  request.request_line.minor_version = 0;
  request.header.host = "example.com";
  request.header.content_type = "text/plain";
  request.header.content_length = 2;
  request.body.value = "ok";

  std::ostringstream output;
  output << request;

  EXPECT_EQ(output.str(),
            "Method: POST\n"
            "Target: /items\n"
            "Protocol: HTTP/1.0\n"
            "Host: example.com\n"
            "Content-Type: text/plain\n"
            "Content-Length: 2\n"
            "Body: ok");
}
