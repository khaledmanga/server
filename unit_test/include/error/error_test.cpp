#include <gtest/gtest.h>
#include <string>
#include "include/error/error.hpp"

TEST(ErrorTest, StrErrorReturnsCorrectMessage) {
  EXPECT_EQ(server_error::strerror(server_error::ErrorCode::SOCKET_CREATION_FAILED), "Socket creation failed");
  EXPECT_EQ(server_error::strerror(server_error::ErrorCode::BIND_FAILED), "Bind failed");
  EXPECT_EQ(server_error::strerror(server_error::ErrorCode::LISTEN_FAILED), "Listen failed");
  EXPECT_EQ(server_error::strerror(server_error::ErrorCode::ACCEPT_FAILED), "Accept failed");
  EXPECT_EQ(server_error::strerror(server_error::ErrorCode::READ_FAILED), "Read failed");
  EXPECT_EQ(server_error::strerror(server_error::ErrorCode::WRITE_FAILED), "Write failed");
}

TEST(ErrorTest, PerrorOutputsCorrectMessage) {
  testing::internal::CaptureStderr();

  server_error::Error error{server_error::ErrorCode::SOCKET_CREATION_FAILED, "Failed to create socket", 1};
  server_error::perror(error);

  std::string output = testing::internal::GetCapturedStderr();
  EXPECT_NE(output.find("[Error 1]: Socket creation failed: Failed to create socket"), std::string::npos);
}
