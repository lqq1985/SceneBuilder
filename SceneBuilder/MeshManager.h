#ifndef MESH_MANAGER
#define MESH_MANAGER

#include <vector>
#include <string>
#include "Mesh.h"
#include "Model.h"

class MeshManager
{
	public:
		MeshManager();
		~MeshManager();
		void loadModel(std::string filename);
		std::vector<Mesh> getMeshes();
	private:
		std::vector<Mesh> meshes;
};

#endif

