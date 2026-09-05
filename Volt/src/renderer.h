#pragma once

#include <iostream>;
#include <format>;
#include <vector>;
#include "game_object.h";
#include "object_registry.h"

namespace Volt {
	struct Grid {
		int w_cells = 50;
		int h_cells = 50;
	};

	class Renderer {
	private:
		Grid grid = Grid{}; // area to clean each frame
		std::string buffer;

	private:
		void render_area();
		void render_horizontal_border(bool isTop);
		void render_vertical_border(bool isLeft);
		void render_objects(ObjectRegistry& registry);

	public:
		Renderer();
		~Renderer();
		void render(ObjectRegistry& registry, int score);
	};
}