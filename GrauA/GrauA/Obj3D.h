#pragma once
#include <GLM/glm.hpp>
#include "Mesh.h"

class Obj3D {
private:
	

public:
	Obj3D();
	~Obj3D();
	void setRenderMode(int i);
	Mesh* mesh;
	string name;
	glm::mat4 transform;
	bool deletable;
	glm::vec3 direction;
	GLuint renderMode;
	string path;
};