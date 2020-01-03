#ifndef ST_VERTEX_H
#define ST_VERTEX_H

#include <glm/glm.hpp>

struct STVertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

#endif