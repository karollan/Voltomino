#pragma once

#include <vector>;
#include "game_object.h";
#include "object_registry.h"

namespace Volt {
	class Physics {
	private:
		int accumulator = 0;
		int gravity_accumulator = 0;
		int gravity_update = 300;
		int physics_update = 10;

	public:
		void tick(int dt, ObjectRegistry& registry);
		void gravity(int dt, ObjectRegistry& registry);
		void tryRotate(GameObject& obj, ObjectRegistry& registry);
	};
}