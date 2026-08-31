#include <gtest/gtest.h>

#include <string>

#include "../src/request.hpp"

TEST(RequestTest, ParseRequestLine) {
    Request request;

    std::string raw = "GET /echo HTTP/1.1";

    parse_request_line(request, raw);

    EXPECT_EQ(request.request_line.method, "GET");
    EXPECT_EQ(request.request_line.path, "/echo");
    EXPECT_EQ(request.request_line.major_version, 1);
    EXPECT_EQ(request.request_line.minor_version, 1);
}

TEST(RequestTest, ParseHeader) {
    Request request;

    std::string raw =
        "Host: localhost:8080\r\n"
        "User-Agent: Mozilla/5.0\r\n"
        "Accept: */*\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 0\r\n"
        "\r\n";

    parse_header(request, raw);

    EXPECT_EQ(request.header.host, "localhost:8080");
    EXPECT_EQ(request.header.user_agent, "Mozilla/5.0");
    EXPECT_EQ(request.header.accept, "*/*");
    EXPECT_EQ(request.header.content_type, "application/json");
    EXPECT_EQ(request.header.content_length, 0);
}

TEST(RequestTest, ParseRequest) {
    Request request;
    RequestState state = RequestState::RequestLine;

    std::string raw =
        "POST /echo HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: Mozilla/5.0\r\n"
        "Accept: */*\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 5\r\n"
        "\r\n"
        "Hello";

    bool result = parse_request(request, state, raw);

    EXPECT_TRUE(result);
    EXPECT_EQ(state, RequestState::Complete);

    EXPECT_EQ(request.request_line.method, "POST");
    EXPECT_EQ(request.request_line.path, "/echo");
    EXPECT_EQ(request.request_line.major_version, 1);
    EXPECT_EQ(request.request_line.minor_version, 1);

    EXPECT_EQ(request.header.host, "localhost:8080");
    EXPECT_EQ(request.header.user_agent, "Mozilla/5.0");
    EXPECT_EQ(request.header.accept, "*/*");
    EXPECT_EQ(request.header.content_type, "text/plain");
    EXPECT_EQ(request.header.content_length, 5);

    EXPECT_EQ(request.body, "Hello");
}
