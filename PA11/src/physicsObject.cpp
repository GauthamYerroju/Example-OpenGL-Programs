#include "physicsObject.hpp"

PhysicsObject::PhysicsObject()
{
	collisionShape = NULL;
	compoundShape = NULL;
	motionState = NULL;
	rigidBody = NULL;
}


PhysicsObject::PhysicsObject( const char *objPath ) : Object::Object( objPath )
{
	collisionShape = NULL;
	compoundShape = NULL;
	motionState = NULL;
	rigidBody = NULL;
}

PhysicsObject::~PhysicsObject()
{
	
	delete collisionShape;
	delete motionState;
	delete rigidBody;
}


/**
 * Initialize a Tirangle Mesh physics object (STATIC)
 * @param  _worldTrans  Rigid transforms from origin with only translation and rotation
						Rotation: btQuaternion(x, y, z, w), Translation: btVector3(x, y, z)
 * @param  _mass        Mass coefficient of object
 * @param  _restitution Ratio of relative velocity after collision to relative velocity before collision
 *                      The coefficient of restitution is in the range [0,1]:[inelastic collision, elastic collision]
 * @param  _friction    Friction coefficient of object 
 *                      In the range of [0,1]:[no friction, high friction]
 * @return              True if physics object succefully created, false otherwise.
 */
bool PhysicsObject::Init_TriMesh(btTransform _worldTrans, btScalar _mass, float _restitution, float _friction)
{
	btCollisionShape *tmp_collisionShape = new btBvhTriangleMeshShape(Get_TriangleMesh(), true);

	if( !tmp_collisionShape )
	{
		printf("collisionShape failed to initialize\n");
		return false;
	}

	return Initialize( tmp_collisionShape, _worldTrans, _mass, _restitution, _friction );
}

/**
 * Initialize a Sphere physics object
 * @param  _worldTrans  Rigid transforms from origin with only translation and rotation
						Rotation: btQuaternion(x, y, z, w), Translation: btVector3(x, y, z)
 * @param  _mass        Mass coefficient of object
 * @param  _restitution Ratio of relative velocity after collision to relative velocity before collision
 *                      The coefficient of restitution is in the range [0,1]:[inelastic collision, elastic collision]
 * @param  _friction    Friction coefficient of object 
 *                      In the range of [0,1]:[no friction, high friction]
 * @param  _radius      Radius of sphere
 * @return              True if physics object succefully created, false otherwise.
 */
bool PhysicsObject::Init_Sphere(btTransform _worldTrans, btScalar _mass, float _restitution, float _friction, float _radius)
{
	btCollisionShape *tmp_collisionShape = new btSphereShape(_radius);

	if( !tmp_collisionShape )
	{
		printf("collisionShape failed to initialize\n");
		return false;
	}

	return Initialize( tmp_collisionShape, _worldTrans, _mass, _restitution, _friction );

}

/**
 * Initialize a Box physics object
 * @param  _worldTrans  Rigid transforms from origin with only translation and rotation
						Rotation: btQuaternion(x, y, z, w), Translation: btVector3(x, y, z)
 * @param  _mass        Mass coefficient of object
 * @param  _restitution Ratio of relative velocity after collision to relative velocity before collision
 *                      The coefficient of restitution is in the range [0,1]:[inelastic collision, elastic collision]
 * @param  _friction    Friction coefficient of object 
 *                      In the range of [0,1]:[no friction, high friction]
 * @param  _halfExtents half extents: btVector3(dx, dy, dz), The full extents of the box will be twice the half extents
 * @return              True if physics object succefully created, false otherwise.
 */
bool PhysicsObject::Init_Box(btTransform _worldTrans, btScalar _mass, float _restitution, float _friction, btVector3 _halfExtents)
{
	btCollisionShape *tmp_collisionShape = new btBoxShape(_halfExtents);

	if( !tmp_collisionShape )
	{
		printf("collisionShape failed to initialize\n");
		return false;
	}

	return Initialize( tmp_collisionShape, _worldTrans, _mass, _restitution, _friction );
}

/**
 * Initialize a Cylinder physics object
 * @param  _worldTrans  Rigid transforms from origin with only translation and rotation
						Rotation: btQuaternion(x, y, z, w), Translation: btVector3(x, y, z)
 * @param  _mass        Mass coefficient of object
 * @param  _restitution Ratio of relative velocity after collision to relative velocity before collision
 *                      The coefficient of restitution is in the range [0,1]:[inelastic collision, elastic collision]
 * @param  _friction    Friction coefficient of object 
 *                      In the range of [0,1]:[no friction, high friction]
 * @param  _halfExtents half extents: btVector3(radius, radius, height). The full extents of the cylinder will be twice the half extents
 * @return              True if physics object succefully created, false otherwise.
 */
bool PhysicsObject::Init_Cylinder(btTransform _worldTrans, btScalar _mass, float _restitution, float _friction, btVector3 _halfExtents)
{
	btCollisionShape *tmp_collisionShape = new btCylinderShapeZ(_halfExtents);

	if( !tmp_collisionShape )
	{
		printf("collisionShape failed to initialize\n");
		return false;
	}

	return Initialize( tmp_collisionShape, _worldTrans, _mass, _restitution, _friction );
}

/**
 * Initialize a StaticPlane physics object
 * @param  _worldTrans  Rigid transforms from origin with only translation and rotation
						Rotation: btQuaternion(x, y, z, w), Translation: btVector3(x, y, z)
 * @param  _mass        Mass coefficient of object
 * @param  _restitution Ratio of relative velocity after collision to relative velocity before collision
 *                      The coefficient of restitution is in the range [0,1]:[inelastic collision, elastic collision]
 * @param  _friction    Friction coefficient of object 
 *                      In the range of [0,1]:[no friction, high friction]
 * @param  _normal      positive direction of the plane
 * @param  _constant    constant with which to project the plane
 * @return              True if physics object succefully created, false otherwise.
 */
bool PhysicsObject::Init_StaticPlane(btTransform _worldTrans, btScalar _mass, float _restitution, float _friction, btVector3 _normal, float _constant)
{
	btCollisionShape *tmp_collisionShape = new btStaticPlaneShape(_normal, _constant);

	if( !tmp_collisionShape )
	{
		printf("collisionShape failed to initialize\n");
		return false;
	}

	return Initialize( tmp_collisionShape, _worldTrans, _mass, _restitution, _friction );
}

/**
 * Initialize a physics object with given collisionShape
 * @param  _collisionShape  A physics collision shape to initialize the phyisics object to
 * @param  _worldTrans  	Rigid transforms from origin with only translation and rotation
							Rotation: btQuaternion(x, y, z, w), Translation: btVector3(x, y, z)
 * @param  _mass        	Mass coefficient of object
 * @param  _restitution 	Ratio of relative velocity after collision to relative velocity before collision
 *                       	The coefficient of restitution is in the range [0,1]:[inelastic collision, elastic collision]
 * @param  _friction    	Friction coefficient of object 
 *                       	In the range of [0,1]:[no friction, high friction]
 * @return                 	True if physics object succefully created, false otherwise.
 */
bool PhysicsObject::Initialize( btCollisionShape *_collisionShape, btTransform _worldTrans, btScalar _mass, float _restitution, float _friction )
{
	collisionShape = _collisionShape;
	motionState = new btDefaultMotionState( _worldTrans );
	if( !motionState){
		printf("motionState failed to initialize\n");
		return false;
	}

	mass = _mass;
	inertia = btVector3(0, 0, 0);

	collisionShape->calculateLocalInertia( mass, inertia );
	btRigidBody::btRigidBodyConstructionInfo constructionInfo( mass, motionState, collisionShape, inertia );

	// Ratio of relative speed after to the realtive speed before the collision
	constructionInfo.m_restitution = _restitution;
	constructionInfo.m_friction = _friction;

	rigidBody = new btRigidBody( constructionInfo );
	if( !rigidBody ){
		printf("rigidBody failed to initialize\n");
		return false;
	}

	return true;
}

bool PhysicsObject::InitializeWithCompoundShape( btCompoundShape *_compoundShape, btTransform _worldTrans, btScalar _mass, float _restitution, float _friction )
{
	compoundShape = _compoundShape;
	motionState = new btDefaultMotionState( _worldTrans );
	if( !motionState ){
		printf("motionState failed to initialize\n");
		return false;
	}

	mass = _mass;
	inertia = btVector3(0, 0, 0);

	compoundShape->calculateLocalInertia( mass, inertia );
	btRigidBody::btRigidBodyConstructionInfo constructionInfo( mass, motionState, compoundShape, inertia );

	// Ratio of relative speed after to the realtive speed before the collision
	constructionInfo.m_restitution = _restitution;
	constructionInfo.m_friction = _friction;

	rigidBody = new btRigidBody( constructionInfo );
	if( !rigidBody ){
		printf("rigidBody failed to initialize\n");
		return false;
	}

	return true;
}



btTriangleMesh * PhysicsObject::Get_TriangleMesh()
{
	btVector3 triArray[3];
	btTriangleMesh *objTriMesh = new btTriangleMesh();

	//While you’re looping through all of the faces in your mesh:
	for(int meshIndx = 0; meshIndx < meshes.size(); meshIndx++)
	{
		int triIndx = 0;
		std::vector<unsigned int> mIndices = meshes[meshIndx].Indices;
		for(int faceIndx = 0; faceIndx < mIndices.size(); faceIndx++)
		{

			glm::vec3 position = meshes[meshIndx].Vertices[mIndices[faceIndx]].vertex;
			triArray[triIndx] = btVector3(position.x, position.y, position.z);

			triIndx++;

			if( triIndx == 3 )
			{
				objTriMesh->addTriangle(triArray[0], triArray[1], triArray[2]);
				triIndx = 0;
			}

		}

	}
	return objTriMesh;
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
