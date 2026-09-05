
#include "window_manager.h"
#include <iostream>
namespace Volt {
	WindowManager::WindowManager() {
		hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hOut == INVALID_HANDLE_VALUE) {
			throw "Console out handle error!";
			return;
		}
		hIn = GetStdHandle(STD_INPUT_HANDLE);
		if (hIn == INVALID_HANDLE_VALUE) {
			throw "Console in handle error!";
			return;
		}

		if (!GetConsoleMode(hOut, &dwOriginalOutMode)) {
			throw "Console can't get original out mode!";
			return;
		}
		if (!GetConsoleMode(hIn, &dwOriginalInMode)) {
			throw "Console can't get original in mode!";
			return;
		}

		dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
		//dwRequestedInModes = ~ENABLE_LINE_INPUT | ~ENABLE_ECHO_INPUT | ~ENABLE_VIRTUAL_TERMINAL_INPUT;

		dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
		if (!SetConsoleMode(hOut, dwOutMode)) {
			throw "Console can't set output mode!";
		}
		dwInMode = dwOriginalInMode | dwRequestedInModes;
		if (!SetConsoleMode(hIn, dwInMode)) {
			throw "Console can't set input mode!";
		}
	}
}