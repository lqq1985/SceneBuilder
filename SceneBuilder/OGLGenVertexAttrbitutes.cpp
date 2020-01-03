#include "OGLGenVertexAttrbitutes.h"

void OglGenVertexAttributes::generate(unsigned int & VAO, unsigned int & VBO, std::vector<STVertex> mesh, std::vector<glm::uvec3> indices, unsigned int attrPosition)
{
	unsigned int IBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(STVertex), &mesh[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec3), glm::value_ptr(indices[0]), GL_STATIC_DRAW);

	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(STVertex), (void*)0);
	glEnableVertexAttribArray(0);

	// normals
	if (attrPosition >= 1) {
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(STVertex), (void*)offsetof(STVertex, normal));
		glEnableVertexAttribArray(1);
	}

	// textures
	if (attrPosition >= 2) {
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(STVertex), (void*)offsetof(STVertex, texCoord));
		glEnableVertexAttribArray(2);
	}

	// tangent
	if (attrPosition >= 3) {
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(STVertex), (void*)offsetof(STVertex, tangent));
		glEnableVertexAttribArray(3);
	}

	// bitangent
	if (attrPosition >= 4) {
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(STVertex), (void*)offsetof(STVertex, bitangent));
		glEnableVertexAttribArray(4);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}
