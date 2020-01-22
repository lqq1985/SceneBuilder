#include "Grid.h"

Grid::Grid() {
	this->shader = new Shader("shaders/grid.vert", "shaders/grid.frag");
	this->setupYAxis();
	this->setupXAxis();
	this->setupZAxis();
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

	glBindVertexArray(this->XVAO);
	glDrawArrays(GL_LINES, 0, 164);
	glBindVertexArray(0);

	glBindVertexArray(this->ZVAO);
	glDrawArrays(GL_LINES, 0, 164);
	glBindVertexArray(0);

	glBindVertexArray(this->YVAO);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

void Grid::setupYAxis()
{
	GLuint VBO, VAO;

	std::vector<glm::vec3> gridData;

	gridData.push_back(glm::vec3(0, -this->lineLength, 0));
	gridData.push_back(glm::vec3(0, this->lineLength, 0));

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &this->YVAO);
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &this->YVAO);
	glBindVertexArray(this->YVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, gridData.size() * sizeof(glm::vec3), &gridData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}

void Grid::setupXAxis()
{
	GLuint VBO, VAO;

	std::vector<glm::vec3> gridData;

	int gridIncrementer = -20;
	for (int i = 0; i <= 40; i++) {
		gridData.push_back(glm::vec3(-this->lineLength, 0, gridIncrementer));
		gridData.push_back(glm::vec3(this->lineLength, 0, gridIncrementer));

		gridIncrementer++;
	}

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &this->XVAO);
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &this->XVAO);
	glBindVertexArray(this->XVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, gridData.size() * sizeof(glm::vec3), &gridData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}

void Grid::setupZAxis()
{
	GLuint VBO, VAO;

	std::vector<glm::vec3> gridData;

	int gridIncrementer = -20;
	for (int i = 0; i <= 40; i++) {
		gridData.push_back(glm::vec3(gridIncrementer, 0, -this->lineLength));
		gridData.push_back(glm::vec3(gridIncrementer, 0, this->lineLength));

		gridIncrementer++;
	}

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &this->ZVAO);
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &this->ZVAO);
	glBindVertexArray(this->ZVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, gridData.size() * sizeof(glm::vec3), &gridData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}
