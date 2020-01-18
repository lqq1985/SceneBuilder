#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>

#include "Shader.h"
#include "Model.h"
#include "Physics.h"

class Scene
{
public:
	Scene();
	~Scene();
	void load();
	void simulate();
	void render(glm::mat4 &projection, glm::mat4 &view, Shader &shader);
private:
	std::vector<Model*> models; 
	Physics* phyics;

	// data
	std::string modelFilenames[1] = {
		"assets/collada_test/collada_test.dae"
		//"assets/scene/scene.obj"
	};

};

#endif
