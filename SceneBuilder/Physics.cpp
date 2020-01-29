#include "Physics.h"

Physics::Physics()
{
	this->collisionConfiguration = new btDefaultCollisionConfiguration();
	this->dispatcher = new btCollisionDispatcher(collisionConfiguration);
	this->overlappingPairCache = new btDbvtBroadphase();
	this->solver = new btSequentialImpulseConstraintSolver;
	this->dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

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

void Physics::simulate(double dt)
{
	dynamicsWorld->stepSimulation(dt);
}

void Physics::addStaticBox(Mesh &mesh, int id)
{

	btTriangleMesh* meshInterface = new btTriangleMesh();

	for (int i = 0; i < mesh.indices.size()/3; i++) { 
		glm::vec3 v1 = glm::vec4(mesh.mTransform * glm::vec4(mesh.vertices[mesh.indices[i * 3]].position, 1.0f));
		glm::vec3 v2 = glm::vec4(mesh.mTransform * glm::vec4(mesh.vertices[mesh.indices[i * 3 + 1]].position, 1.0f));
		glm::vec3 v3 = glm::vec4(mesh.mTransform * glm::vec4(mesh.vertices[mesh.indices[i * 3 + 2]].position, 1.0f));

		meshInterface->addTriangle(btVector3(v1.x, v1.y, v1.z),
			btVector3(v2.x, v2.y, v2.z),
			btVector3(v3.x, v3.y, v3.z));
	}

	btBvhTriangleMeshShape* groundShape = new btBvhTriangleMeshShape(meshInterface, true, true);

	this->collisionShapes.push_back(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();

	btScalar mass(0.);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		groundShape->calculateLocalInertia(mass, localInertia);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	body->setUserIndex(id);

	//add the body to the dynamics world
	this->dynamicsWorld->addRigidBody(body);
}

void Physics::addDynamicBox(Mesh &mesh, int id)
{
	//create a dynamic rigidbody
	// btBoxShape vectors must all be positive
	btCollisionShape* colShape = new btBoxShape(btVector3(btScalar(mesh.extents.x), btScalar(mesh.extents.y), btScalar(mesh.extents.z)));

	this->collisionShapes.push_back(colShape);

	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();
	//startTransform.setFromOpenGLMatrix(glm::value_ptr(mesh.mTransform));

	btScalar mass(1.f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic) 
		colShape->calculateLocalInertia(mass, localInertia);

	
	 startTransform.setOrigin(btVector3(mesh.position.x, mesh.position.y, mesh.position.z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	body->setUserIndex(id);

	this->dynamicsWorld->addRigidBody(body);
}

void Physics::getUpdatedPositions(std::vector<Mesh> &meshes)
{
	for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		btScalar matrix[16];
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

		if (!body->isStaticObject()) {
			trans.getOpenGLMatrix(matrix);
			meshes[obj->getUserIndex()].mTransform = UtilConversion::btScalar2mat4(matrix);
		}
	}
}

void Physics::drawDebugData(glm::mat4 projection, glm::mat4 view)
{
	this->debugDrawerOpenGL.SetMatrices(projection, view);

	this->dynamicsWorld->debugDrawWorld();
}
