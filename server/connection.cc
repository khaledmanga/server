#include "connection.h"

#include <iostream>
#include <string>
#include <unistd.h>
#include "http/response/response.h"

Connection::Connection(int client_fd) {
    this->client_fd = client_fd;
}

bool Connection::handle_read() {
    HttpStatus http_status = HttpStatus::RequestLine;
    std::string raw_request;

    while(http_status != HttpStatus::End) {
        const ssize_t bytes_read = read(this->client_fd, this->read_buffer, sizeof(this->read_buffer));
        if(bytes_read <= 0) {
            return false;
        }

        raw_request.append(this->read_buffer, static_cast<size_t>(bytes_read));
        http_parser(this->request, http_status, raw_request);
        if(http_status == HttpStatus::Error) {
            return false;
        }
    }

    return true;
}

void Connection::handle_write() {
    Response response;
    response.body.value = "OK\n";
    response.header.kv.insert_or_assign("Content-Length", std::to_string(response.body.value.size()));
    response.header.kv.insert_or_assign("Content-Type", "text/plain");

    const std::string output = response.ok();
    size_t bytes_written = 0;
    while(bytes_written < output.size()) {
        const ssize_t count = write(
            this->client_fd, output.data() + bytes_written, output.size() - bytes_written);
        if(count <= 0) {
            std::cerr << "Failed to write HTTP response\n";
            return;
        }
        bytes_written += static_cast<size_t>(count);
    }
}

void Connection::run() {
    if(this->handle_read()) {
        this->handle_write();
    }
    close(this->client_fd);
}