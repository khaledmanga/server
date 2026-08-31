#include <gtest/gtest.h>

#include "../src/response.hpp"

TEST(ResponseTest, StatusLine) {
  StatusLine status_line;

  status_line.setVersion("HTTP/1.1")
      .setStatusCode(200)
      .setReasonPhrase("OK");

  EXPECT_EQ(status_line.version(), "HTTP/1.1");
  EXPECT_EQ(status_line.statusCode(), 200);
  EXPECT_EQ(status_line.reasonPhrase(), "OK");
}

TEST(ResponseTest, Headers) {
  Headers headers;

  headers.setContentType("text/plain")
      .setContentLength(5)
      .setDate("Mon, 31 Aug 2026 14:00:00 GMT")
      .setServer("MyServer/1.0")
      .setConnection("keep-alive");

  EXPECT_EQ(headers.contentType(), "text/plain");
  EXPECT_EQ(headers.contentLength(), 5);
  EXPECT_EQ(headers.date(), "Mon, 31 Aug 2026 14:00:00 GMT");
  EXPECT_EQ(headers.server(), "MyServer/1.0");
  EXPECT_EQ(headers.connection(), "keep-alive");
}

TEST(ResponseTest, HeadersDateIsSetByDefault) {
  Headers headers;

  EXPECT_FALSE(headers.date().empty());
}

TEST(ResponseTest, Response) {
  Response response;

  StatusLine status_line;
  status_line.setVersion("HTTP/1.1")
      .setStatusCode(200)
      .setReasonPhrase("OK");

  Headers headers;
  headers.setContentType("text/plain")
      .setServer("MyServer/1.0")
      .setConnection("close");

  response.setStatusLine(status_line)
      .setHeaders(headers)
      .setBody("Hello");

  EXPECT_EQ(response.statusLine().version(), "HTTP/1.1");
  EXPECT_EQ(response.statusLine().statusCode(), 200);
  EXPECT_EQ(response.statusLine().reasonPhrase(), "OK");

  EXPECT_EQ(response.headers().contentType(), "text/plain");
  EXPECT_EQ(response.headers().server(), "MyServer/1.0");
  EXPECT_EQ(response.headers().connection(), "close");

  EXPECT_EQ(response.body(), "Hello");
  EXPECT_EQ(response.headers().contentLength(), 5);
}
