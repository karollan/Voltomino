
#include "voltomino.h";
#include <math.h>
namespace Volt {
	Voltomino::Voltomino
		(const Transform& t, const Mesh& mesh, const Collider& collider, const RigidBody& rb)
		: GameObject(t, mesh, collider, rb, 2)
	{
		
	}

}