#include <gtest/gtest.h>

#include <stdexcept>

#include "../src/server.hpp"

TEST(ServerTest, RequiresDependenciesBeforeRunning) {
  Server server(0);

  EXPECT_THROW(server.run(), std::logic_error);
}