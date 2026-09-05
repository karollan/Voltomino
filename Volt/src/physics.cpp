#include "physics.h";
#include <math.h>
namespace Volt {
	void Physics::tick(int dt, ObjectRegistry& registry) {
		gravity(dt, registry);

		// pretty bad collision detection but taking into account
		// amount of objects and mesh sizes its ok
		for (const auto& obj : registry.objects) {
			//collision
			// // in our case this matters because without collision event there is no effect
			if (obj->collider.enabled) {
				// n^2
				for (const auto& against : registry.objects) {
					if (&against != &obj && against->collider.enabled && against->collider.layer == obj->collider.layer) {
						std::vector<Cell> b = against->getCellsAt(against->transform.x, against->transform.y);
						int nextX = obj->transform.x + obj->rb.deltaX;
						int nextY = obj->transform.y + obj->rb.deltaY;
						std::vector<Cell> a = obj->getCellsAt(nextX, nextY);
						std::vector<Cell> overlapping;
						overlapping.reserve(obj->mesh.cells.capacity());

						// brute force, lists won't have more then 20 elements
						// hashing is overhead in this case
						for (const auto& a_cell : a) {
							for (const auto& b_cell : b) {
								if (a_cell.x == b_cell.x && a_cell.y == b_cell.y) {
									overlapping.emplace_back(a_cell.x, a_cell.y);
								}
							}
						}
						if (overlapping.empty()) {
							continue;
						}
						if (obj->rb.deltaX != 0) {
							if (obj->rb.deltaX > 0) {
								obj->onCollision(against, CollisionSide::right);
							} else {
								obj->onCollision(against, CollisionSide::left);
							}
							obj->rb.deltaX = 0; // no movement
						}
						if (obj->rb.deltaY != 0) {
							if (obj->rb.deltaY > 0) {
								obj->onCollision(against, CollisionSide::bottom);
							}
							else {
								obj->onCollision(against, CollisionSide::top);
							}
							obj->rb.deltaY = 0; // no movement
						}
					}
				}
			}


			// apply movement
			if (obj->rb.rotationDelta == 1) {
				tryRotate(*obj, registry);
				obj->rb.rotationDelta = 0;
			}

			obj->transform.x += obj->rb.deltaX;
			obj->transform.y += obj->rb.deltaY;
			obj->rb.deltaX = 0;
			obj->rb.deltaY = 0;
		}
	}

	static const int KICKS[][2] = {
		{0,0},
		{-1,0},{1,0},
		{0,-1},
		{-1,-1},{1,-1},
		{-3,0},{3,0}
	};

	static Box getBoundingBox(std::vector<Cell>& cells) {
		int lowestX = cells[0].x;
		int highestX = cells[0].x;
		int lowestY = cells[0].y;
		int highestY = cells[0].y;
		for (const auto& cell : cells) {
			if (cell.x > highestX) highestX = cell.x;
			if (cell.x < lowestX) lowestX = cell.x;
			if (cell.y > highestY) highestY = cell.y;
			if (cell.y < lowestY) lowestY = cell.y;
		}
		return Box{ lowestX, lowestY, highestX, highestY, highestX - lowestX + 1, highestY - lowestY + 1 };
	}

	static void rotate90(std::vector<Cell>& cells) {
		Box box = getBoundingBox(cells);
		for (auto& cell : cells) {
			int xt = box.minX + (box.maxY - cell.y);
			int yt = box.minY + (cell.x - box.minX);
			cell.x = xt;
			cell.y = yt;
		}
	}

	static bool collides(ObjectRegistry& registry, GameObject& obj, std::vector<Cell>& cells, int x, int y) {
		if (obj.collider.enabled) {
			for (const auto& against : registry.objects) {
				if (&(*against) != &obj && against->collider.enabled && against->collider.layer == obj.collider.layer) {
					std::vector<Cell> b = against->getCellsAt(against->transform.x, against->transform.y);
					std::vector<Cell> overlapping;
					overlapping.reserve(cells.capacity());
					for (const auto& a_cell : cells) {
						for (const auto& b_cell : b) {
							int a_cellX = a_cell.x + x + obj.transform.x;
							int a_cellY = a_cell.y + y + obj.transform.y;
							if (a_cellX == b_cell.x && a_cellY == b_cell.y) {
								overlapping.emplace_back(a_cellX, a_cellY);
							}
						}
					}
					if (!overlapping.empty()) {
						return true;
					}
				}
			}
		}
		return false;
	}

	void Physics::tryRotate(GameObject& obj, ObjectRegistry& registry) {
		std::vector<Cell> trial = obj.mesh.cells;
		rotate90(trial);
		for (auto [kx, ky] : KICKS) {
			if (!collides(registry, obj, trial, kx, ky)) {
				obj.mesh.cells = std::move(trial);
				obj.transform.x += kx;
				obj.transform.y += ky;
				return;
			}
		}
	}

	void Physics::gravity(int dt, ObjectRegistry& registry) {
		gravity_accumulator += dt;

		if (gravity_accumulator < gravity_update) {
			return;
		}

		for (const auto& obj : registry.objects) {
			obj->rb.deltaY = obj->rb.gravity;
		}
		gravity_accumulator = 0;
	}
}