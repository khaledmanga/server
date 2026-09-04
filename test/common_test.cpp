#include <gtest/gtest.h>

#include <regex>

#include "../src/common.hpp"

TEST(CommonTest, CurrentTimeUsesExpectedFormat) {
  const std::string current_time = getCurrentTime();

  EXPECT_TRUE(std::regex_match(
      current_time, std::regex(R"(\d{2}/\d{2}/\d{4} \d{2}:\d{2}:\d{2})")));
}