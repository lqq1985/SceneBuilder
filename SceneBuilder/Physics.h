#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "GLDebugDrawer.h"
#include <vector>

class Physics
{
	public:
		Physics();
		~Physics();
		void simulate(double dt);
		void addStaticBox(glm::vec3 position, glm::vec3 extents);
		void addDynamicBox(glm::vec3 position, glm::vec3 extentt);
		void drawDebugData(glm::mat4 projection, glm::mat4 view);
		btAlignedObjectArray<btCollisionShape*> collisionShapes;
	private:
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;
		BulletDebugDrawer_OpenGL debugDrawerOpenGL;
};

#endif