#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include "graphics_headers.hpp"

class PhysicsWorld
{
	public:
		PhysicsWorld();
		~PhysicsWorld();

		void Initialize();
		void AddRigidBody( btRigidBody *rBody );
		void Update(unsigned int dt);

		void setGravity( glm::vec3 g );

	private:
		//	Tells bullet how to check for collisions between objects. This will also help eliminate object pairs that should not collide.
    	btBroadphaseInterface					*broadphase;
    	// This is a collision algorithm.
    	// It can be used to register a callback that filters overlapping broadphase proxies
    	// so that the collisions are not processed by the rest of the system.
		btCollisionConfiguration				*collisionConfig;
		// Takes in the collisionConfiguration pointer as a parameter.
		// Used in conjunction with collisionConfiguration for the collision algorithm.
		// It will send events to the objects.
		btCollisionDispatcher					*dispatcher;
		// This causes the objects to interact properly, taking into account gravity, forces, collisions, and hinge constraints.
		// Both the world and the objects are handled with this pointer.
		btSequentialImpulseConstraintSolver		*solver;

		//Takes in all of the parameters you just created.
		btDiscreteDynamicsWorld					*dynamicsWorld;
};


#endif /* PHYSICSWORLD_H */