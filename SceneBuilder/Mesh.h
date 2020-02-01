#ifndef MESH_H
#define MESH_H

#include <assimp/Importer.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <vector>
#include <string>
#include <string>
#include "Shader.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
	public:
		Mesh(
			std::vector<Vertex> vertices,
			std::vector<unsigned int> indices,
			std::vector<Texture> textures,
			glm::vec3 extents,
			std::string name,
			glm::mat4 mTransform
		);
		~Mesh();
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		glm::vec3 extents;
		glm::vec3 position;
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::mat4 mTransform;
		std::string name;
		int meshType;

		unsigned int VAO, VBO, EBO;
	private:
		void setup();
};

#endif

