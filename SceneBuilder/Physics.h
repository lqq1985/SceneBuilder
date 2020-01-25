#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "GLDebugDrawer.h"
#include <vector>

#include "Mesh.h"

class Physics
{
	public:
		Physics();
		~Physics();
		void simulate(double dt);
		void addStaticBox(Mesh mesh, glm::vec3 extents);
		void addDynamicBox(Mesh mesh, glm::vec3 extents);
		void getUpdatedPositions(std::vector<glm::vec3>& newPositions);
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