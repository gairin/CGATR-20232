#include "Obj3D.h"
#include <vector>

Obj3D::Obj3D() {
	Mesh* mesh;
	glm::mat4 transform;
	int status;
	bool deletable;
	std::vector<glm::vec3> path;
};

Obj3D::~Obj3D() {}

Mesh* Obj3D::getMesh() {
	return this->mesh;
}

void Obj3D::setMesh(Mesh* mesh) {
	this->mesh = mesh;
}

void Obj3D::setTransform(glm::mat4 t) {
	this->transform = t;
}