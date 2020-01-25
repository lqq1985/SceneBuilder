#include "Physics.h"

Physics::Physics()
{
	this->collisionConfiguration = new btDefaultCollisionConfiguration();
	this->dispatcher = new btCollisionDispatcher(collisionConfiguration);
	this->overlappingPairCache = new btDbvtBroadphase();
	this->solver = new btSequentialImpulseConstraintSolver;
	this->dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -1, 0));

	this->dynamicsWorld->setDebugDrawer(&this->debugDrawerOpenGL);
	this->dynamicsWorld->getDebugDrawer()->setDebugMode(3);


	this->collisionShapes;
}

Physics::~Physics()
{
	for (int i = this->dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = this->dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		this->dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < this->collisionShapes.size(); j++)
	{
		btCollisionShape* shape = this->collisionShapes[j];
		this->collisionShapes[j] = 0;
		delete shape;
	}

	delete this->dynamicsWorld;
	delete this->solver;
	delete this->overlappingPairCache;
	delete this->dispatcher;
	delete this->collisionConfiguration;
}

void Physics::simulate()
{
	dynamicsWorld->stepSimulation(1.f / 60.f, 10);

	//print positions of all objects
	for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}

		//printf("world pos object %d = %f,%f,%fd\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	}
}

void Physics::addBoxShape(glm::vec3 position, glm::vec3 extents, bool hasMass)
{
	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(extents.x), btScalar(extents.y), btScalar(extents.z)));

	this->collisionShapes.push_back(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(position.x, position.y, position.z));

	btScalar mass = hasMass ? 1.0f : 0.f;
	
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		groundShape->calculateLocalInertia(mass, localInertia);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	//add the body to the dynamics world
	this->dynamicsWorld->addRigidBody(body);
}

void Physics::drawDebugData(glm::mat4 projection, glm::mat4 view)
{
	this->debugDrawerOpenGL.SetMatrices(projection, view);

	this->dynamicsWorld->debugDrawWorld();
}
