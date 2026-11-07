#include <string>

class Connection {
    public:
        Connection(int socket_fd);
        ~Connection();

        std::string read_buffer;
        std::string write_buffer;
        int socket_fd;

        void handle_read();
        void handle_write();
}