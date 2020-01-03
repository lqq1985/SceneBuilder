#include "Sphere.h"

void Sphere::create(std::vector<STVertex>& mesh, std::vector<glm::uvec3>& indices, unsigned int stacks, unsigned int slices)
{
	const float PI = 3.14;

	// vertices
	for (int i = 0; i <= stacks; ++i)
	{
		float v = (float)i / (float)stacks;
		float phi = v * PI;

		for (int j = 0; j <= slices; ++j)
		{
			float u = (float)j / (float)slices;
			float theta = u * (PI * 2);

			float x = cos(theta) * sin(phi);
			float y = cos(phi);
			float z = sin(theta) * sin(phi);

			STVertex tMesh;

			tMesh.position = glm::vec3(x, y, z);
			mesh.push_back(tMesh);
		}
	}

	// indices
	for (int i = 0; i < slices * stacks + slices; ++i)
	{
		indices.push_back(glm::uvec3(
			i,
			i + slices + 1,
			i + slices
		));

		indices.push_back(glm::uvec3(
			i + slices + 1,
			i,
			i + 1
		));
	}
}
