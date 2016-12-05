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

		bool Init_TriMesh(btTransform _worldTrans, btScalar _mass, float _restitution, float _friction);
		bool Init_Sphere(btTransform _worldTrans, btScalar _mass, float _restitution, float _friction, float _radius);
		bool Init_Box(btTransform _worldTrans, btScalar _mass, float _restitution, float _friction, btVector3 _halfExtents);
		bool Init_Cylinder(btTransform _worldTrans, btScalar _mass, float _restitution, float _friction, btVector3 _halfExtents);
		bool Init_StaticPlane(btTransform _worldTrans, btScalar _mass, float _restitution, float _friction, btVector3 _normal, float _constant);
		bool Initialize( btCollisionShape *_collisionShape, btTransform _worldTrans, btScalar _mass, float _restitution, float _friction );
		void Update();
		btTriangleMesh * Get_TriangleMesh();
		btTransform GetWorldTransform();
		btRigidBody * GetRigidBody();
		btCollisionShape * GetShape();

	protected:
		btDefaultMotionState	*motionState;
		btCollisionShape 		*collisionShape;
		btRigidBody				*rigidBody;

		btScalar				mass;
		btVector3				inertia;
	
};

#endif /* PHYSICCSOBJECT_H */