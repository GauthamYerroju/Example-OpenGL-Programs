#ifndef PHYSICCSOBJECT_H
#define PHYSICCSOBJECT_H

#include "graphics_headers.hpp"

class PhysicsObject
{
	public:
		PhysicsObject();
		~PhysicsObject();

		enum CollisionShapeType 
		{
			BOX_SHAPE = 1,
			SPHERE_SHAPE = 2,
		};

		bool Initialize( CollisionShapeType shape, btScalar m, btQuaternion rotation, btVector3 translation );
		btTransform GetWorldTransform();
		btRigidBody * GetRigidBody();

	private:
		btCollisionShape 		*collisionShape;
		btDefaultMotionState	*motionState;
		btRigidBody				*rigidBody;

		btScalar				mass;
		btVector3				inertia;



};

#endif /* PHYSICCSOBJECT_H */