#include "server.h"

Server::Server() {
    this->running = true;
}

~Server::Server() {
    close(this->server_fd);
}

void Server::set_port(int port) {
    this->port = port;
}

void Server::setup_socket() {
    this->server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(this->port);

    this->bind_socket(addr);
}

void Server::bind_socket(struct sockaddr_in& addr) {
    bind(this->server_fd, (struct sockaddr*)&addr, sizeof(addr));
}

void Server::listen_sockets() {
    listen(this->fd, MAXSOCONN);
}

void Server::accept_client() {
    struct sockaddr_in client;
    socklen_t client_len = sizeof(client); 

    int fd_client = accept(fd_server, (struct sockaddr*)&client, (socklen_t*)&client_len);

    Connection connection(fd_client);
    connection.run();
}

void Server::run() {
    this->setup_socket();
    this->listen_sockets();

    while(this->running) {
        this->accpet_client();
    }
}