#include "physicsObject.hpp"


PhysicsObject::PhysicsObject( const char *objPath ) : Object::Object( objPath )
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
			collisionShape = new btSphereShape(0.5);
			break; 
		case BOX_SHAPE:
			// Static Plane with normal (0, 1, 0) and planeConstant of 0.5
			collisionShape = new btStaticPlaneShape( btVector3(0, 1, 0), 0.5 );
			break;
		default:
			printf("collisionShape failed to init\n");
			return false;
	}

		
	// btTransform: rigid transforms with only translation and rotation
	// Rotation: btQuaternion(x, y, z, w), Translation: btVector3(x, y, z) 
	motionState = new btDefaultMotionState( btTransform(rotation, translation) ); 
	if( !motionState){
		printf("motionState failed to init\n");
		return false;
	}

	mass = m;
	inertia = btVector3(0, 0, 0);

	collisionShape->calculateLocalInertia( mass, inertia );	
	btRigidBody::btRigidBodyConstructionInfo constructionInfo( mass, motionState, collisionShape, inertia );

	// Ratio of relative speed after to the realtive speed bofore the collision
	constructionInfo.m_restitution = 0.9f;
	constructionInfo.m_friction = 1.5f;

	rigidBody = new btRigidBody( constructionInfo );
	if( !rigidBody ){
		printf("rigidBody failed to init\n");
		return false;
	}

	return true;
}


void PhysicsObject::Update()
{
	btTransform trans;
	btScalar m[16]; 
	 
	rigidBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(m); 
	model = glm::make_mat4(m);
}

btTransform PhysicsObject::GetWorldTransform()
{
	btTransform trans;
	rigidBody->getMotionState()->getWorldTransform(trans);

	return trans;
}


btRigidBody * PhysicsObject::GetRigidBody()
{
	return rigidBody;
}