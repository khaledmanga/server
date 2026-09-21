#include <gtest/gtest.h>

#include <atomic>
#include <stdexcept>

#include "../src/server.hpp"

TEST(ServerTest, RequiresDependenciesBeforeRunning) {

  std::atomic<bool> shutdown_requested{false};

  Server server(0, shutdown_requested);

  EXPECT_THROW(server.run(), std::logic_error);
}
