#pragma once

#include <windows.h>

namespace Volt {

	class WindowManager {
	private:
		HANDLE hOut = INVALID_HANDLE_VALUE;
		HANDLE hIn = INVALID_HANDLE_VALUE;

		DWORD dwOriginalOutMode = 0;
		DWORD dwOriginalInMode = 0;

		DWORD dwRequestedOutModes = 0;
		DWORD dwRequestedInModes = 0;

		DWORD dwOutMode = 0;
		DWORD dwInMode = 0;

	public:
		WindowManager();
		HANDLE inputHandle() const { return hIn; }
		HANDLE outputHandle() const { return hOut; }

		WindowManager(const WindowManager&) = delete;
		WindowManager& operator=(const WindowManager&) = delete;
	};

}