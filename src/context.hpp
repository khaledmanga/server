#include <atomic>

class AppContext {
private:
	AppContext() = default;
	AppContext(const AppContext&) = delete;
	AppContext& operator=(const AppContext&) = delete;
	AppContext(const AppContext&&) = delete;
	AppContext& operator=(const AppContext&&) = delete;
public:
	std::atomic<bool> shutting_down{false};
}
