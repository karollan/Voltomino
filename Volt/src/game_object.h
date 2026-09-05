#pragma once

#include <vector>;
#include <functional>;
#include <windows.h>

namespace Volt {

	struct Cell {
		int x;
		int y;
	};

	struct Box {
		int minX;
		int minY;
		int maxX;
		int maxY;
		int width;
		int height;
	};

	struct Transform {
		int x;
		int y;
	};

	struct RigidBody {
		int gravity;
		int deltaX = 0;
		int deltaY = 0;
		int rotationDelta = 0;
	};

	struct Mesh {
		std::vector<Cell> cells;
		char glyph = '#';
		bool enabled = true;
	};

	struct Collider {
		int layer;
		bool enabled;
	};

	enum CollisionSide {
		top,
		left,
		right,
		bottom
	};

	class GameObject {
	public:
		Transform transform;
		Mesh mesh;
		Collider collider;
		RigidBody rb;
		int type = 0;
		std::function<void(GameObject*, CollisionSide)> onCollision;
		std::function<void(WORD)> onInput;

	public:
		GameObject() = default;
		GameObject(const Transform& t, const Mesh& mesh, const Collider& collider, const RigidBody& rb, int type);
		~GameObject();

		std::vector<Cell> getCellsAt(int x, int y) {
			std::vector<Cell> result;
			result.reserve(mesh.cells.capacity());
			for (const auto& cell : mesh.cells) {
				result.emplace_back(cell.x + x, cell.y + y);
			}
			return result;
		}
	};

}