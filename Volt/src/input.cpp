#include "input.h";

namespace Volt {
	Input::Input(WindowManager& manager) : window(manager)
	{
	}

	void Input::pollInput(ObjectRegistry& registry) {
		DWORD numEvents = 0;
		if (!GetNumberOfConsoleInputEvents(window.inputHandle(), &numEvents)) {
			throw "Failed while getting input events";
			return;
		}

		if (numEvents > 0) {
			const DWORD maxEvents = 64;
			INPUT_RECORD buffer[maxEvents];
			DWORD eventsRead = 0;
			if (ReadConsoleInput(window.inputHandle(), buffer, min(numEvents, maxEvents), &eventsRead)) {
				for (DWORD i = 0; i < eventsRead; ++i) {
					if (buffer[i].EventType == KEY_EVENT) {
						KEY_EVENT_RECORD& key = buffer[i].Event.KeyEvent;
						bool down = key.bKeyDown;
						WORD vk = key.wVirtualKeyCode;

						if (down) {
							for (const auto& obj : registry.objects) {
								if (obj->onInput) {
									obj->onInput(vk);
								}
							}
						}
					}
				}
			}
		}
	}
}
