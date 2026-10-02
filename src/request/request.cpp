#include "request.hpp"

std::ostream& operator << (std::ostream& os, const RequestLine &rl) {
	os << "Method: " << rl.method << std::endl
	   << "Target: " << rl.target << std::endl
	   << "Protocol: " << rl.protocol << "/" << rl.major_version << "." << rl.minor_version;
	
	return os;
}

std::ostream& operator << (std::ostream& os, const Header &h) {
	os << "Host: " << h.host << std::endl
	   << "Content-Type: " << h.content_type << std::endl
	   << "Content-Length: " << h.content_length;
	
	return os;
}

std::ostream& operator << (std::ostream& os, const Body &b) {
	os << "Body: " << b.value;
	
	return os;
}

std::ostream& operator << (std::ostream& os, const Request &r) {
	os << r.request_line << std::endl << r.header << std::endl << r.body;

	return os;
}

