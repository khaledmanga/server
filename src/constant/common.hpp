#pragma once

#include <string>
#include <iostream>

enum class HttpRequestState {
	RequestLine,
	Headers,
	Body,
	Completed,
};

enum class HttpMethod {
	GET,
	POST,
	DELETE,
	PATCH,
	PUT,
};

std::string to_string(HttpMethod method);