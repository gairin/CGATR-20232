#include "Obj3D.h"
#include <vector>

Obj3D::Obj3D() {
	this->transform = glm::mat4(1.0f);
};

Obj3D::~Obj3D() {

}

void Obj3D::setRenderMode(int i) {
	i == 0 ? this->renderMode = GL_TRIANGLES : this->renderMode = GL_QUADS;
}