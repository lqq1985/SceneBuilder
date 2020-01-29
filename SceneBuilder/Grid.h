#ifndef GRID_H
#define GRID_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"

class Grid
{
	public:
		Grid();
		~Grid();
		void render(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos);
	private:
		int gridSize = 18;
		int numLines;
		GLuint VAO;
		Shader* shader;
		void setupAxis();
};

#endif

