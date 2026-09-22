#include <gtest/gtest.h>

#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>

#include "../src/logger.hpp"

class RecordingSink : public Sink {
 public:
  void write(const LogRecord& record) override {
    {
      std::lock_guard lock(mutex_);

      level = record.level();
      message = record.msg();
      timestamp = record.timestamp();
    }


    cv_.notify_one();
  }

  bool waitForRecord() {
    std::unique_lock lock(mutex_);

    return cv_.wait_for(
        lock,
        std::chrono::seconds(1),
        [this] {
          return !message.empty();
        });
  }

  LogLevel level = LogLevel::DEBUG;
  std::string message;

  std::string timestamp;

 private:
  std::mutex mutex_;
  std::condition_variable cv_;
};

TEST(LoggerTest, SendsRecordToRegisteredSink) {
  Logger logger;

  auto sink = std::make_unique<RecordingSink>();

  auto* sink_ptr = sink.get();


  logger.addSink(std::move(sink));

  logger.log(LogLevel::WARN, "database unavailable");

  ASSERT_TRUE(sink_ptr->waitForRecord());

  EXPECT_EQ(sink_ptr->level, LogLevel::WARN);
  EXPECT_EQ(sink_ptr->message, "database unavailable");
  EXPECT_FALSE(sink_ptr->timestamp.empty());
}
