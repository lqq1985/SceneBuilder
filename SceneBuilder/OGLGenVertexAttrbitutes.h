#ifndef OGL_GEN_VERTEX_ATTRIBUTES_H
#define OGL_GEN_VERTEX_ATTRIBUTES_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "STVertex.h"

namespace OglGenVertexAttributes {
	void generate(unsigned int& VAO, unsigned int& VBO, std::vector<STVertex> mesh, std::vector<glm::uvec3> indices, unsigned int attrPosition);
}

#endif