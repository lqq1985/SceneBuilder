#include "MeshManager.h"

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
}

void MeshManager::loadModel(std::string filename, int meshType)
{
	Model m = Model(filename);

	for (int i = 0; i < m.meshes.size(); i++) {
		m.meshes[i].meshType = meshType;
		this->meshes.push_back(m.meshes[i]);
	}

	// this->meshes.insert(this->meshes.end(), m.meshes.begin(), m.meshes.end());
}

std::vector<Mesh> MeshManager::getMeshes()
{
	return this->meshes;
}
