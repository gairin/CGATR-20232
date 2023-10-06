#pragma once
#include <GLM/glm.hpp>
#include "Mesh.h"

class Obj3D {
private:
	Mesh* mesh;
	string name;
	glm::mat4 transform;
	bool deletable;
	glm::vec3 direction;
	string path;

public:
	Obj3D();
	~Obj3D();
	Mesh* getMesh();
	void setName(string name);
	void setDeletable(bool d);
	void setMesh(Mesh* mesh);
	void setTransform(glm::mat4 t);
	void setPath(string path);
	glm::mat4 getTransform();
};