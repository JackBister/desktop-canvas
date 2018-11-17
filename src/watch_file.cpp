#include "watch_file.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <filesystem>

struct CallbackParam {
	HANDLE watchHandle;
	std::function<void()> callback;

	CallbackParam(HANDLE watchHandle, std::function<void()> callback) : watchHandle(watchHandle), callback(callback) {}
};

void watch_file_callback(PVOID lpParameter, BOOLEAN TimerOrWaitFired) {
	auto arg = (CallbackParam *)lpParameter;
	arg->callback();
	FindNextChangeNotification(arg->watchHandle);
}

void watch_file_W32(std::wstring filename, std::function<void()> onChange) {
	std::filesystem::path path(filename);
	auto absolutePath = std::filesystem::absolute(path).parent_path();

	auto watchHandle = FindFirstChangeNotification(absolutePath.c_str(), false, FILE_NOTIFY_CHANGE_LAST_WRITE);
	HANDLE newWaitObject;

	void * callbackArgs = new CallbackParam(watchHandle, onChange);
	auto registerResult = RegisterWaitForSingleObject(
		&newWaitObject,
		watchHandle,
		watch_file_callback,
		callbackArgs,
		INFINITE,
		WT_EXECUTEDEFAULT
		);
	
	if (registerResult == 0) {
		printf("[ERROR] Couldn't register file watcher, error code %x", GetLastError());
		return;
	}
}

#endif

void dcanvas::watch_file(std::wstring filename, std::function<void()> onChange) {
#ifdef _WIN32
	watch_file_W32(filename, onChange);
#endif
}
