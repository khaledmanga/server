#pragma once

#include <atomic>

class AppContext {
private:
	AppContext() = default;
	AppContext(const AppContext&) = delete;
	AppContext& operator=(const AppContext&) = delete;
	AppContext(AppContext&&) = delete;
	AppContext& operator=(AppContext&&) = delete;
public:
	inline static std::atomic<bool> shutting_down{false};
};
