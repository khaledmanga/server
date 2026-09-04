#include "common.hpp"

std::string getCurrentTime() {
  time_t rawtime;
  time(&rawtime);
  struct tm* timeinfo = localtime(&rawtime);

  char buffer[80];

  strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", timeinfo);

  return std::string(buffer);
}
