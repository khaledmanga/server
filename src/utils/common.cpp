#include "common.hpp"

bool isValidMethodHttp(const std::string& http_method) {
	if(http_method == to_string(HttpMethod::GET) || http_method == to_string(HttpMethod::POST)) {
		return true;
	}
	
	return false;
}