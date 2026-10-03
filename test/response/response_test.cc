#include "../../src/response/response.h"

#include <gtest/gtest.h>

#include <string>

TEST(ResponseTest, SerializesDefaultOkResponse) {
  Response response;

  EXPECT_EQ(response.serialize(),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain; charset=utf-8\r\n"
            "Content-Length: 0\r\n"
            "Connection: close\r\n"
            "\r\n");
}

TEST(ResponseTest, SerializesStatusAndBody) {
  Response response;
  response.status(HTTP::StatusCode::CREATED).send("created");

  EXPECT_EQ(response.serialize(),
            "HTTP/1.1 201 Created\r\n"
            "Content-Type: text/plain; charset=utf-8\r\n"
            "Content-Length: 7\r\n"
            "Connection: close\r\n"
            "\r\n"
            "created");
}

TEST(ResponseTest, SendReturnsResponseForChaining) {
  Response response;
  Response &sent_response =
      response.status(HTTP::StatusCode::NOT_FOUND).send("missing");

  EXPECT_EQ(&sent_response, &response);
  EXPECT_EQ(response.serialize().substr(0, 24), "HTTP/1.1 404 Not Found\r\n");
}

TEST(ResponseTest, ContentLengthCountsBodyBytes) {
  Response response;
  response.send("\xC3\xA9");

  EXPECT_NE(response.serialize().find("Content-Length: 2\r\n"),
            std::string::npos);
}
