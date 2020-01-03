#include "Light.h"

Light::Light()
{
}


Light::~Light()
{
}

void Light::load(glm::vec3 position)
{
	this->position = position;

	Sphere::create(this->mesh, this->indices, 16, 16);
	OglGenVertexAttributes::generate(this->VAO, this->VBO, this->mesh, this->indices, 0);
}

void Light::draw(glm::mat4 & projection, glm::mat4 & view, Shader & shader)
{
	shader.use();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);

	glBindVertexArray(this->VAO);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, this->position);
	model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
	shader.setMat4("model", model);

	glDrawElements(GL_TRIANGLES, (GLsizei)this->indices.size() * 3, GL_UNSIGNED_INT, 0);
}

void Light::gui()
{
	ImGui::Begin("Light");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	ImGui::Text("Position");
	ImGui::SliderFloat("X", &this->position.x, -32.0f, 32.0f);
	ImGui::SliderFloat("Y", &this->position.y, -32.0f, 32.0f);
	ImGui::SliderFloat("Z", &this->position.z, -32.0f, 32.0f);
	ImGui::SetWindowPos(ImVec2(0, 500), true);
	ImGui::End();
}
