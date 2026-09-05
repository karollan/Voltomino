#pragma once

#include "game_manager.h";
#include "window_manager.h";
#include "input.h";
#include "renderer.h";
#include "physics.h";
#include <chrono>
#include <array>

namespace Volt {
	class Clock {
	private:
		int last_frame_time = 0;
		int dt = 0;
	public:
		int current_time();
		void update();
		int get_dt();
	};

	class Game {
	private: 
		bool running = false;
	public:
		void start();
	};
}