#pragma once

#include <iostream>

enum class HttpRequestState {
	RequestLine,
	Headers,
	Body,
	Completed,
};
