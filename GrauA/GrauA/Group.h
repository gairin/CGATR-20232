#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Face.h"

using namespace std;

class Group {
private:
	

public:
	Group();
	~Group();
	string name;
	string material;
	vector<Face*> faces;
	GLuint VAO;
	int numVertices;
};