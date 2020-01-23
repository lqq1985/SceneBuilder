#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "OGLGenTexture.h"
#include "Shader.h"
#include "STModel.h"

class Model
{
	public:
		Model(std::string filename);
		~Model();
		void loadModel(std::string filename);
		void draw(glm::mat4 &projection, glm::mat4 &view, Shader &shader);
		std::vector<Mesh> meshes;
	private:
		std::string fileLocation;
		std::string directory;
		std::vector<Texture> texturesLoaded;
		void processNode(aiNode *node, const aiScene *scene, aiMatrix4x4 transformation);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 transformation);
		std::vector<Vertex> getVertices(aiMesh* mesh, glm::vec3 &extents, glm::vec3 &origin);
		std::vector<unsigned int> getIndices(aiMesh* mesh);
		std::vector<Texture> getTexures(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadTextureFiles(aiMaterial* mat, aiTextureType type, std::string typeName);
		glm::mat4 aiMatrix4x4ToGlm(aiMatrix4x4 from);
}; 

#endif

