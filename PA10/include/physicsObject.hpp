#ifndef PHYSICCSOBJECT_H
#define PHYSICCSOBJECT_H

#include "graphics_headers.hpp"
#include "object.hpp"

class PhysicsObject : public Object
{
	public:
		PhysicsObject();
		PhysicsObject( const char *objPath );
		~PhysicsObject();

		enum CollisionShapeType
		{
			TRIANGLE_MESH = 1,
			BOX_SHAPE = 2,
			SPHERE_SHAPE = 3,
			CYLINDER_SHAPE = 4,
			STATIC_PLANE_SHAPE = 5
		};

		bool Initialize( CollisionShapeType shape, btScalar m, btTransform worldTrans, float restitution, float friction );
		void Update();
		btTriangleMesh * Get_TriangleMesh();
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