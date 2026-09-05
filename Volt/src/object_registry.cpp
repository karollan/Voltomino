#include "object_registry.h"

namespace Volt {

	ObjectRegistry::ObjectRegistry() {
		objects.reserve(50);
	}
	ObjectRegistry::~ObjectRegistry() {
		objects.clear();
	}
}