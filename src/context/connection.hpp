#pragma once

#include <sys/socket.h>

#include "../http_parser/http_parser.hpp"

class Connection {
	private:
		int fd;
		Request request;
		std::string read_buffer;
		std::string write_buffer;
			
	public:
		Connection(int fd_);
		void handle_read();
		void handle_write();
};
