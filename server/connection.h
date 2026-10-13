#pragma once

#include <string>


class Connection {
    private:
        int client_fd;
        std::string read_buffer;
        std::string write_buffer;
        void handle_read();
        void handle_write();
    
    public:
        Connection(int);
        void run();
}
