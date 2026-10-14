#pragma once

#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include "connection.h"

class Server {
    private:
        int port;
        int server_fd;
        bool running;
        void setup_socket();
        void bind_socket(struct sockaddr_in&);
        void listen_sockets();
        void accept_client();
    public:
        Server();
        void set_port(int port);
        void run();
        ~Server();
};