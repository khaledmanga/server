#include "http_parser.hpp"

/**
	POST /api/users?active=true&page=2 HTTP/1.1\r\n
	Host: localhost:8080\r\n
	User-Agent: curl/8.5.0\r\n
	Accept: application/json\r\n
	Content-Type: application/json\r\n
	Authorization: Bearer abc123\r\n
	Connection: keep-alive\r\n
	Content-Length: 47\r\n
	\r\n
	{
	  "username": "kha",
	  "email": "kha@example.com"
	}
*/

HttpRequestState httpParser(Request& request, HttpRequestState& httpRequestState, const std::string& raw_request) {
    if (httpRequestState == HttpRequestState::RequestLine) {
        std::string request_line = raw_request.substr(0, raw_request.find("\r\n"));

        std::istringstream iss(request_line);

        std::string method;
        std::string target;
        std::string protocol_version;

        iss >> method >> target >> protocol_version;

        request.request_line.method = method;
        request.request_line.target = target;

        auto slash = protocol_version.find('/');
        auto dot = protocol_version.find('.');

        request.request_line.protocol = protocol_version.substr(0, slash);
        request.request_line.major_version = std::stoi(protocol_version.substr(slash + 1,dot - slash - 1));
        request.request_line.minor_version = std::stoi(protocol_version.substr(dot + 1));
    }

    return httpRequestState;
}