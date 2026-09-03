#include <functional>
#include <sys/epoll.h>

class Channel {
public:
  using Callback = std::function<void()>;

private:
  Callback read_callback_;
  Callback write_callback_;
  uint32_t event_;
  int fd_;

public:
  Channel(int fd, uint32_t event);
  int fd() const;
  uint32_t event() const;
  void setReadCallback(Callback callback);
  void setWriteCallback(Callback callback);
  void handleEvent(uint32_t event);
};
