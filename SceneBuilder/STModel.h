#ifndef ST_MODEL_H
#define ST_MODEL_H

#include <glm/glm.hpp>

struct STModel {
	glm::vec3 position;
	glm::vec3 scale = glm::vec3(1,1,1);
};

#endif