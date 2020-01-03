#ifndef SPHERE_H
#define SPHERE_H

#include "STVertex.h"
#include <vector>

namespace Sphere {
	void create(std::vector<STVertex>& mesh, std::vector<glm::uvec3>& indices, unsigned int stacks, unsigned int slices);
}

#endif