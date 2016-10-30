#include "physicsObject.hpp"


PhysicsObject::PhysicsObject()
{
	collisionShape = NULL;
	motionState = NULL;
	rigidBody = NULL;

}


PhysicsObject::~PhysicsObject()
{
	delete collisionShape;
	delete motionState;
	delete rigidBody;

}

bool PhysicsObject::Initialize( CollisionShapeType shape, btScalar m, btQuaternion rotation, btVector3 translation )
{
	switch(shape)
	{
		case SPHERE_SHAPE:
			// Sphere with radius 0.5
			printf("SPHERE_SHAPE\n");
			collisionShape = new btSphereShape(0.5);
			break; 
		case BOX_SHAPE:
			// Static Plane with origin offset (0, 0, 0) and planeConstant of 1
			printf("BOX_SHAPE\n");
			collisionShape = new btStaticPlaneShape( btVector3(0, 0, 0), 1 );
		default:
			return false;
	}

	if( !collisionShape )
		printf("collisionShape failed to init\n");

	// btTransform: rigid transforms with only translation and rotation
	// Rotation: btQuaternion(x, y, z, w), Translation: btVector3(x, y, z) 
	motionState = new btDefaultMotionState( btTransform(rotation, translation) ); 
	if( !motionState)
		printf("motionState failed to init\n");

	mass = m;
	inertia = btVector3(0, 0, 0);
	
	collisionShape->calculateLocalInertia( mass, inertia );	
	btRigidBody::btRigidBodyConstructionInfo constructionInfo( mass, motionState, collisionShape, inertia );

	rigidBody = new btRigidBody( constructionInfo );

	if( !rigidBody )
		printf("rigidBody failed to init\n");

	return true;
}


btTransform PhysicsObject::GetWorldTransform()
{
	btTransform trans;
	printf("Check3.1-------\n");
	rigidBody->getMotionState()->getWorldTransform(trans);

	return trans;
}


btRigidBody * PhysicsObject::GetRigidBody()
{
	return rigidBody;
}