#include "common.hpp"

std::string to_string(HttpMethod method) {
    switch (method) {
        case HttpMethod::GET:    return "GET";
        case HttpMethod::POST:   return "POST";
        case HttpMethod::PUT:    return "PUT";
        case HttpMethod::DELETE: return "DELETE";
        default:                 return "UNKNOWN";
    }
}

namespace HTTP {

    std::string getReasonPhrase(StatusCode code) {
        switch (code) {
            case StatusCode::OK:                    return "OK";
            case StatusCode::CREATED:               return "Created";
            case StatusCode::NO_CONTENT:            return "No Content";
            
            case StatusCode::MOVED_PERMANENTLY:     return "Moved Permanently";
            case StatusCode::FOUND:                 return "Found";
            
            case StatusCode::BAD_REQUEST:           return "Bad Request";
            case StatusCode::UNAUTHORIZED:          return "Unauthorized";
            case StatusCode::FORBIDDEN:             return "Forbidden";
            case StatusCode::NOT_FOUND:             return "Not Found";
            
            case StatusCode::INTERNAL_SERVER_ERROR: return "Internal Server Error";
            case StatusCode::NOT_IMPLEMENTED:       return "Not Implemented";
            case StatusCode::BAD_GATEWAY:           return "Bad Gateway";
            
            default:                                return "Unknown Status";
        }
    }

}