#include "request.hpp"

std::ostream& operator << (std::ostream& os, const RequestLine &rl) {
	os << "Method: " << rl.method << std::endl
	   << "Target: " << rl.target << std::endl
	   << "Protocol: " << rl.protocol << "/" << rl.major_version << "." << rl.minor_version;
	
	return os;
}

std::ostream& operator << (std::ostream& os, const Request &r) {
	os << r.request_line << std::endl;

	return os;
}

