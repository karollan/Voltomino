#include "game.h"

namespace Volt {
	int Clock::current_time() {
		using namespace std::chrono;

		return duration_cast<milliseconds>(
			steady_clock::now().time_since_epoch()
		).count();
	}
	void Clock::update() {
		dt = current_time() - last_frame_time;
		last_frame_time = current_time();
	}
	int Clock::get_dt() {
		return dt;
	}

	void Game::start() {
		running = true;

		WindowManager window;

		ObjectRegistry registry;

		Clock clock;
		Input input{ window };
		Physics physics;
		Renderer renderer;

		GameManager gameManager{ registry };

		while (running) {
			if (gameManager.isGameOver()) {
				gameManager.newGame();
			}

			input.pollInput(registry);
			clock.update();
			physics.tick(clock.get_dt(), registry);

			gameManager.update();

			renderer.render(registry, gameManager.score);
		}
	}
}
