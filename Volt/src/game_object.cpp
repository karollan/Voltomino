#include "game_object.h"

namespace Volt {
	GameObject::GameObject(const Transform& t, const Mesh& mesh, const Collider& collider, const RigidBody& rb, int type)
		: transform(t), mesh(mesh), collider(collider), rb(rb), type(type)
	{
	}
	GameObject::~GameObject() {

	}
}