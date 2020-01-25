#ifndef RENDER_H
#define RENDER_H

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include "Mesh.h"
#include "Shader.h"

namespace Render
{
	void mesh(std::vector<Mesh> meshes, glm::mat4 projection, glm::mat4 view, Shader shader);
};

#endif
