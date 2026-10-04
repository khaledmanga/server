#pragma once

#include <cstring>
#include <iostream>
#include <sstream>

#include "../constant/common.h"
#include "../request/request.h"
#include "../utils/common.h"

HttpRequestState httpParser(Request &request, HttpRequestState &httpRequestState, std::string &raw_request);
