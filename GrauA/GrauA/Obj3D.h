#pragma once
#include <GLM/glm.hpp>
#include "Mesh.h"

class Obj3D {
private:
	

public:
	Obj3D();
	~Obj3D();
	Mesh* mesh;
	string name;
	glm::mat4 transform;
	bool deletable;
	glm::vec3 direction;
	string path;
};