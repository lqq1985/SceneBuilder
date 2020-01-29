#include "Grid.h"

Grid::Grid() {
	this->shader = new Shader("shaders/grid.vert", "shaders/grid.frag");
	this->setupAxis();
}

Grid::~Grid() {
	delete this->shader;
}

void Grid::render(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos) {
	this->shader->use();
	this->shader->setMat4("projection", projection);
	this->shader->setMat4("view", view);
	this->shader->setVec3("colour", glm::vec3(0.15, 0.15, 0.15));
	this->shader->setVec3("viewPos", viewPos);

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_LINES, 0, this->numLines);
	glBindVertexArray(0);
}

void Grid::setupAxis()
{
	GLuint VBO, VAO;

	std::vector<glm::vec3> gridData;
	int gridIncrementer;

	// Y Axis
	gridData.push_back(glm::vec3(0, -this->gridSize, 0));
	gridData.push_back(glm::vec3(0, this->gridSize, 0));

	// X Axis
	gridIncrementer = -this->gridSize;
	for (int i = 0; i <= this->gridSize * 2; i++) {
		gridData.push_back(glm::vec3(-this->gridSize, 0, gridIncrementer));
		gridData.push_back(glm::vec3(this->gridSize, 0, gridIncrementer));

		gridIncrementer++;
	}

	// Z Axis
	gridIncrementer = -this->gridSize;
	for (int i = 0; i <= this->gridSize * 2; i++) {
		gridData.push_back(glm::vec3(gridIncrementer, 0, -this->gridSize));
		gridData.push_back(glm::vec3(gridIncrementer, 0, this->gridSize));

		gridIncrementer++;
	}

	this->numLines = gridData.size();

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, gridData.size() * sizeof(glm::vec3), &gridData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}