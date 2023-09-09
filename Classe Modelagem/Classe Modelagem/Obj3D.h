#pragma once
#include <GLM/glm.hpp>

#include "Mesh.h"

class Obj3D {
	private:
		Mesh* mesh;
		glm::mat4 transform;
		int status;
		bool deletable;
		glm::vec3 direction;

	public:
		Obj3D() {

		};

		~Obj3D() {

		};
};