#include "connection.h"

#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

Connection::Connection(int client_fd) {
    this->client_fd = client_fd;
}