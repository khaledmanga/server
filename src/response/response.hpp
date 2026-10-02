#pragma once

#include <iostream>
#include <cstring>

#include "../constant/common.hpp"

class StatusLine {
public:
	std::string protocol;
	int major_version;
	int minor_version;
	int status_code;
	std::string reason;
}

class Headers {
public:
	std::string content-type;
	int content_length;
}

class Body {
public:
	std::string value;
}

class Response {
public:
	StatusLine status_line;
	Headers header;
	Body body;
	
	Response(HTTP::StatusCode status_code);
}