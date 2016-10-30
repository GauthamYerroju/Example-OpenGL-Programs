#ifndef PHYSICCSOBJECT_H
#define PHYSICCSOBJECT_H

class PhysicsObject
{
	public:
		PhysicsObject();
		~PhysicsObject();

		btRigidBody * GetRigidBody();

	private:
		btCollisionShape 		*collisionShape;
		btDefaultMotionState	*motionState;
		btRigidBody				*rigidBody;

		btScaler				mass;
		btVector3				intertia;


};

#endif /* PHYSICCSOBJECT_H */