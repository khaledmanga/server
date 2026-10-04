#include "server.h"

#include <fcntl.h>
#include <sys/signalfd.h>

#include <cerrno>
#include <exception>
#include <system_error>

#include "../thread_pool/thread_pool.h"

namespace {
int check(int rc, const char *what) {
  if (rc == -1) throw std::system_error(errno, std::generic_category(), what);
  return rc;
}
}  // namespace

Server::Server(int port_, const Router &router_, Logger &logger_, ThreadPool &thread_pool_)
    : logger(logger_), thread_pool(thread_pool_), port(port_), server_fd(-1), signal_fd(-1), router(router_) {}

void Server::setupSignals() {
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGINT);
  sigaddset(&set, SIGTERM);

  if (int err = pthread_sigmask(SIG_BLOCK, &set, nullptr)) {
    throw std::system_error(err, std::generic_category(), "pthread_sigmask");
  }
  signal_fd = check(signalfd(-1, &set, SFD_NONBLOCK | SFD_CLOEXEC), "signalfd");
}

void Server::setupSocket() {
  server_fd = check(socket(AF_INET, SOCK_STREAM, 0), "socket");

  int opt = 1;
  check(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)), "setsockopt");

  int flags = fcntl(server_fd, F_GETFL, 0);
  check(fcntl(server_fd, F_SETFL, flags | O_NONBLOCK), "fcntl");

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  check(bind(server_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)), "bind");
  check(listen(server_fd, SOMAXCONN), "listen");
}

void Server::acceptClient() {
  int client_fd = accept(server_fd, nullptr, nullptr);
  if (client_fd == -1) {
    perror("Accept socket");
    return;
  }

  int flags = fcntl(client_fd, F_GETFL, 0);
  check(fcntl(client_fd, F_SETFL, flags | O_NONBLOCK), "fcntl");

  event_loop.add_event(client_fd);
  pending_clients.insert(client_fd);
}

void Server::handleClient(int fd) {
  if (fd == signal_fd) {
    signalfd_siginfo info{};
    if (read(signal_fd, &info, sizeof(info)) != sizeof(info)) {
      throw std::system_error(errno, std::generic_category(), "read signalfd");
    }
    std::cout << "Server is shutting down..." << std::endl;
    event_loop.remove_event(server_fd);
    event_loop.stop();
    return;
  }

  pending_clients.erase(fd);
  event_loop.remove_event(fd);
  thread_pool.enqueue([this, fd]() {
    try {
      Connection(fd, router, logger).handle_read();
    } catch (const std::exception &e) {
      logger.Error(std::string("Client handling failed: ") + e.what());
    }
    close(fd);
  });
}

void Server::gracefulShutdown() {
  for (int *fd : {&server_fd, &signal_fd}) {
    if (*fd != -1) {
      close(*fd);
      *fd = -1;
    }
  }
  for (int fd : pending_clients) {
    event_loop.remove_event(fd);
    close(fd);
  }
  pending_clients.clear();
  thread_pool.shutdown();
}

void Server::run() {
  try {
    setupSignals();
    setupSocket();
    event_loop.add_event(signal_fd);

    std::cout << "Server is listening on port " << port << std::endl;
    event_loop.run(server_fd, [this]() { acceptClient(); }, [this](int fd) { handleClient(fd); });
  } catch (...) {
    gracefulShutdown();
    throw;
  }
  gracefulShutdown();
}