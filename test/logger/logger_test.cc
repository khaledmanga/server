#include "../../src/logger/logger.h"

#include <gtest/gtest.h>

#include <string>

template <typename Fn>
void expectLog(Fn log, const std::string &level) {
  Logger logger;
  const std::string message = "logger test message";

  testing::internal::CaptureStdout();
  log(logger, message);
  const std::string out = testing::internal::GetCapturedStdout();

  EXPECT_NE(out.find("[" + level + "]"), std::string::npos);
  EXPECT_NE(out.find(message), std::string::npos);
}

TEST(LoggerTest, Info) {
  expectLog([](Logger &l, const std::string &m) { l.Info(m); }, "INFO");
}

TEST(LoggerTest, Debug) {
  expectLog([](Logger &l, const std::string &m) { l.Debug(m); }, "DEBUG");
}

TEST(LoggerTest, Error) {
  expectLog([](Logger &l, const std::string &m) { l.Error(m); }, "ERROR");
}

TEST(LoggerTest, Warn) {
  expectLog([](Logger &l, const std::string &m) { l.Warn(m); }, "WARN");
}

TEST(LoggerTest, LogWritesRequestedLevel) {
  expectLog([](Logger &l, const std::string &m) { l.log(Log::Level::DEBUG, m); }, "DEBUG");
}