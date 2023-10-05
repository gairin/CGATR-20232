#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Face.h"

using namespace std;

class Group {
private:
	string name;
	string material;
	vector<Face*> faces;
	GLuint VAO;

public:
	Group();
	~Group();
	void setName(string name);
	void setMaterial(string material);
	string getMaterial();
	void addFaces(Face* face);
	vector<Face*> getFaces();
	void setVAO(GLuint vao);
	GLuint getVAO();
	int numVertices;
};