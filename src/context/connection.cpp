#include "connection.hpp"

Connection::Connection(int fd_) {
	this->fd = fd_;
}

void Connection::handle_read() {
    this->read_buffer.resize(4096);

    ssize_t bytes = recv(this->fd, this->read_buffer.data(), this->read_buffer.size(), 0);


    if (bytes <= 0) return;

    this->read_buffer.resize(bytes);
	
	HttpRequestState httpRequestState = HttpRequestState::RequestLine;
	
	HttpRequestState state = httpParser(this->request, httpRequestState, this->read_buffer);
	
	std::cout << this->request << std::endl;
}

void Connection::handle_write() {
	if (this->write_buffer.empty()) return;
	
	ssize_t bytes = send(this->fd, this->write_buffer.data(), this->write_buffer.size(), 0);
	
	if (bytes <= 0) return;
	
	this->write_buffer.erase(0, bytes);
}
