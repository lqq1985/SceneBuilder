#ifndef ST_MATERIAL_H
#define ST_MATERIAL_H

#include <glm/glm.hpp>

struct STMaterial {
	float shininess = 32.0f;
	glm::vec3 specular = glm::vec3(0.1f, 0.1f, 0.1f);
};

#endif