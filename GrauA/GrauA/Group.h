#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Face.h"

using namespace std;

class Group {
private:
	std::string name;
	std::string material;
	vector<Face*> faces;
	GLuint vertVAO;
	GLuint texCoordVAO;
	GLuint normVAO;

public:
	Group();
	~Group();
	void setName(string name);
	void setMaterial(string material);
	void addFaces(Face* face);
	vector<Face*> getFaces();
	void setVertVAO(GLuint vao);
	GLuint getVertVAO();
	void setTexCoordVAO(GLuint vao);
	GLuint getTexCoordVAO();
	void setNormVAO(GLuint vao);
	GLuint getNormVAO();
};