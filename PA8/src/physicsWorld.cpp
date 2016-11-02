#include "physicsWorld.hpp"


PhysicsWorld::PhysicsWorld()
{
	broadphase = NULL;
	collisionConfig = NULL;
	dispatcher = NULL;
	solver = NULL;
	dynamicsWorld = NULL;
}


PhysicsWorld::~PhysicsWorld()
{
	if( dynamicsWorld ){
		delete dynamicsWorld;
		dynamicsWorld = NULL;
	}
	if( solver ){
		delete solver;
		solver = NULL;
	}
	if( dispatcher ){
		delete dispatcher;
		dispatcher = NULL;
	}
	if( collisionConfig ){
		delete collisionConfig;
		collisionConfig = NULL;
	}
	if( broadphase ){
		delete broadphase;
		broadphase = NULL;
	}
}


void PhysicsWorld::Initialize()
{
	broadphase = new btDbvtBroadphase();
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);

	// Set the gravity
	setGravity( glm::vec3(0.0, 0.0, 9.81) );
}

void PhysicsWorld::AddRigidBody( btRigidBody *rBody )
{
	dynamicsWorld->addRigidBody( rBody );
}

void PhysicsWorld::Update(unsigned int dt)
{
	dynamicsWorld->stepSimulation(float(dt)/1000, 5);
}

void PhysicsWorld::setGravity( glm::vec3 g )
{
	if( !dynamicsWorld ){
		printf("Failed to set gravity: Dynamics World not initialized\n");
		return;
	}
	dynamicsWorld->setGravity( btVector3(g.x, g.y, g.z) );

}
