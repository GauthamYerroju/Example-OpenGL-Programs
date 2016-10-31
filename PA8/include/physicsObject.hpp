#ifndef PHYSICCSOBJECT_H
#define PHYSICCSOBJECT_H

#include "graphics_headers.hpp"
#include "object.hpp"

class PhysicsObject : public Object
{
	public:
		PhysicsObject( const char *objPath );
		~PhysicsObject();

		enum CollisionShapeType 
		{
			BOX_SHAPE = 1,
			SPHERE_SHAPE = 2,
		};

		bool Initialize( CollisionShapeType shape, btScalar m, btQuaternion rotation, btVector3 translation );
		void Update(); 
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