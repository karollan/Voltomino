#include "renderer.h";

namespace Volt {

	Renderer::Renderer() {
		printf("\x1b[" "?1049h");
		printf("\x1b[?25l");
		fflush(stdout);
	}

	Renderer::~Renderer() {
		printf("\x1b[?25h");
		printf("\x1b[?1049l");
		fflush(stdout);
	}

	void Renderer::render_horizontal_border(bool isTop) {
		if (isTop) {
			buffer += "\x1b[0;0H";
		}
		else {
			buffer += std::format("\x1b[{};0H", grid.h_cells + 2);
		}
		buffer += "\x1b(0";
		buffer += isTop ? "l" : "m";
		for (int i = 0; i < grid.w_cells; i++) {
			buffer += "q";
		}
		buffer += isTop ? "k" : "j";
		buffer += "\x1b[0m";
		buffer += "\x1b(B";
	}

	void Renderer::render_vertical_border(bool isLeft) {
		buffer += "\x1b(0";
		for (int i = 1; i < grid.h_cells + 1; ++i) {
			buffer += std::format("\x1b[{};{}H", i + 1, isLeft ? 0 : grid.w_cells + 2);
			buffer += "x";
		}
		buffer += "\x1b(B";
	}

	void Renderer::render_area() {
		render_horizontal_border(true);
		render_horizontal_border(false);
		render_vertical_border(true);
		render_vertical_border(false);
	}

	void Renderer::render_objects(ObjectRegistry& registry) {
		for (const auto& obj : registry.objects) {
			if (obj->mesh.enabled) {
				for (const auto& cell : obj->mesh.cells) {
					int x = obj->transform.x + cell.x;
					int y = obj->transform.y + cell.y;
					buffer += std::format("\x1b[{};{}H{}", y + 1, x + 1, obj->mesh.glyph);
				}
			}
		}
	}

	void Renderer::render(ObjectRegistry& registry, int score) {
		buffer += "\x1b[H";
		buffer += "\x1b[0E";
		for (int height = 0; height < grid.h_cells; ++height) {
			buffer += "\x1b[0C";
			for (int width = 0; width < grid.w_cells; ++width) {
				buffer += " ";
			}
			buffer += "\x1b[0E";
		}

		render_objects(registry);
		buffer += std::format("\x1b[2;15HScore: {}", score);

		fwrite(buffer.data(), 1, buffer.size(), stdout);
		fflush(stdout);
		buffer.clear();
	}
}