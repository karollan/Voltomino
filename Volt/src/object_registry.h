#pragma once

#include <vector>
#include "game_object.h"

namespace Volt {
	class ObjectRegistry {

	public:
		std::vector<GameObject*> objects;

	public:
		ObjectRegistry();
		~ObjectRegistry();
		void RegisterObject(GameObject* obj) {
			objects.push_back(obj);
		}
		void UnregisterObject(GameObject* obj) {
			auto registered = std::find(objects.begin(), objects.end(), obj);
			if (registered != objects.end()) {
				objects.erase(registered);
			}
		}
	};
}