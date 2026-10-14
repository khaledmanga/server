#include "request.h"

std::optional<std::string> Header::operator[](const std::string& key) const {
    auto it = this->kv.find(key);

    if(it == this->kv.end()) {
        return std::nullopt;
    }

    return it->second;
}