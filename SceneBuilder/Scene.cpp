#include "Scene.h"

Scene::Scene()
{
	this->phyics = new Physics();
}

Scene::~Scene()
{
	delete this->phyics;
	for (int i = 0; i < this->models.size(); i++) delete this->models[i];
}

void Scene::load()
{
	// Why are the models loading incorrectly?	
	// When the blender mesh file vertices are loaded, the vertex points are in their world locations. Therefore, setting
	// the translation with the origin position, moves the vertices away from theire origin position.
	// When setting the phyiscs obejcts, we are using extents which are in object space. Using the origin position therefore
	// moves them in the correct position.
	// Now we need to fiture out how ot translate between the two.

	// Load models
	for (int i = 0; i < sizeof(this->modelFilenames)/sizeof(this->modelFilenames[0]); i++) {
		Model* m = new Model(this->modelFilenames[i]);

		for (int j = 0; j < m->meshes.size(); j++) {
			// this->phyics->addBoxShape(glm::vec3(1,1,1), m->meshes[j].extents, false);
			// this->phyics->addBoxShape(m->meshes[j].origin, m->meshes[j].extents, false);
		}

		this->models.push_back(m);
	}
}

void Scene::simulate()
{
	std::vector<glm::vec3> objects;

	this->phyics->simulate(objects);
}

void Scene::render(glm::mat4& projection, glm::mat4& view, Shader& shader)
{
	for (int i = 0; i < this->models.size(); i++) {
		this->models[i]->draw(projection, view, shader);
	}

	this->phyics->drawDebugData(projection, view);
}
