#pragma once

#include <windows.h>
#include "window_manager.h"
#include <iostream>
#include <iomanip>
#include "game_object.h"
#include "object_registry.h"

namespace Volt {

	class Input {
		
	private:
		WindowManager& window;

	public:
		Input(WindowManager& window);
		void pollInput(ObjectRegistry& registry);
	};
}