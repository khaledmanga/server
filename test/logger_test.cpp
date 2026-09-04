#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "../src/logger.hpp"

class RecordingSink : public Sink {
public:
  void write(const LogRecord &record) override {
    level = record.level();
    message = record.msg();
    timestamp = record.timestamp();
  }

  LogLevel level = LogLevel::DEBUG;
  std::string message;
  std::string timestamp;
};

TEST(LoggerTest, SendsRecordToRegisteredSink) {
  Logger logger;
  auto sink = std::make_unique<RecordingSink>();
  RecordingSink *sink_ptr = sink.get();
  logger.addSink(std::move(sink));

  logger.log(LogLevel::WARN, "database unavailable");

  EXPECT_EQ(sink_ptr->level, LogLevel::WARN);
  EXPECT_EQ(sink_ptr->message, "database unavailable");
  EXPECT_FALSE(sink_ptr->timestamp.empty());
}