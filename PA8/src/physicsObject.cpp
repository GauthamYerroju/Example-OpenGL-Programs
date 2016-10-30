#include "physicsObject"


PhysicsObject::PhysicsObject( std::string cShape )
{
	// Hard coded radius
	collisionShape = new btSphereShape(1); 
	// btTransform: rigid transforms with only translation and rotation
	// Rotation: btQuaternion(x, y, z, w), Translation: btVector3(x, y, z) 
	motionState = new btDefaultMotionState( btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 3, 0)) ); 
	
	mass = 1;
	intertia = btVector3(0, 0, 0);
	collisionShape->calculateLocalInertia( mass, inertia );
	//
	btRigidBody::btRigidBodyConstructionInfo constructionInfo( mass, motionState, collisionShape, inertia );

	rigidBody = new btRigidBody( constructionInfo );

}


PhysicsObject::~PhysicsObject()
{
	delete collisionShape;
	delete motionState;
	delete rigidBody;

}


btRigidBody * GetRigidBody::GetRigidBody()
{
	return rigidBody;
}