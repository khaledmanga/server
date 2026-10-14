#include "common.h"

int toStatusCode(StatusCode status_code) {
    switch (status_code)
    {
    case StatusCode::OK:
        return 200;
    case StatusCode::NotFound:
        return 404;
    }
    return 500;
}

std::string toStatusMessage(StatusCode status_code) {
     switch (status_code)
    {
    case StatusCode::OK:
        return "OK";
    case StatusCode::NotFound:
        return "Not found";
    }
    return "Internal Server Error";
}