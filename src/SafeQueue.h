#pragma once

#include <mutex>
#include <queue>

template<typename T>
class SafeQueue {
public:

	T pop() {
		std::lock_guard guard(lock);
		auto ret = queue.front();
		queue.pop();
		return ret;
	}

	void push(T val) {
		std::lock_guard guard(lock);
		queue.push(val);
	}

	size_t size() {
		std::lock_guard guard(lock);
		return queue.size();
	}

private:
	std::mutex lock;
	std::queue<T> queue;
};
