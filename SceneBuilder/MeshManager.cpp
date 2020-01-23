#include "MeshManager.h"

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
}

void MeshManager::loadModel(std::string filename)
{
	Model m = Model(filename);

	this->meshes.insert(this->meshes.end(), m.meshes.begin(), m.meshes.end());
}

std::vector<Mesh> MeshManager::getMeshes()
{
	return this->meshes;
}
