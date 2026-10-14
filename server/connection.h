#pragma once

#include <string>

#include "http/request/request.h"
#include "http/http_parser/http_parser.h"
#include "include/common.h"

class Connection {
    private:
        int client_fd;
        char read_buffer[4096];
        Request request;
        bool handle_read();
        void handle_write();
    
    public:
        Connection(int);
        void run();
};
