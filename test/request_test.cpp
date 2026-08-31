#include "request.hpp"
#include <gtest/gtest.h>

TEST(RequestTest, ParseRequestLine) {
    Request request;
    std::string buffer = "GET /echo HTTP/1.1\r\n";

    parse_request_line(request, buffer);

    EXPECT_EQ(request.request_line.method, "GET");
    EXPECT_EQ(request.request_line.path, "/echo");
    EXPECT_EQ(request.request_line.major_version, 1);
    EXPECT_EQ(request.request_line.minor_version, 1);
}
