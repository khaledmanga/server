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
  header.fields["Host"] = "example.com";
  header.fields["Content-Type"] = "text/plain";
  header.fields["Content-Length"] = "5";

  std::ostringstream output;
  output << header;

  EXPECT_NE(output.str().find("Host: example.com\n"), std::string::npos);
  EXPECT_NE(output.str().find("Content-Type: text/plain\n"), std::string::npos);
  EXPECT_NE(output.str().find("Content-Length: 5\n"), std::string::npos);
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
  request.header.fields["Host"] = "example.com";
  request.header.fields["Content-Type"] = "text/plain";
  request.header.fields["Content-Length"] = "2";
  request.body.value = "ok";

  std::ostringstream output;
  output << request;

  const std::string serialized = output.str();
  EXPECT_NE(serialized.find("Method: POST\n"
                            "Target: /items\n"
                            "Protocol: HTTP/1.0\n"),
            std::string::npos);
  EXPECT_NE(serialized.find("Host: example.com\n"), std::string::npos);
  EXPECT_NE(serialized.find("Content-Type: text/plain\n"), std::string::npos);
  EXPECT_NE(serialized.find("Content-Length: 2\n"), std::string::npos);
  EXPECT_NE(serialized.find("Body: ok"), std::string::npos);
}
