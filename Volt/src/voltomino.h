#pragma once

#include "game_object.h"

namespace Volt {

	enum VoltominoState {
		PLAYING,
		PLACED,
		READY,
		NOT_INITIALIZED
	};
	
	class Voltomino : public GameObject {

	public:
		VoltominoState state = NOT_INITIALIZED;
	public:
		Voltomino(const Transform& t, const Mesh& mesh, const Collider& collider, const RigidBody& rb);
	};
}