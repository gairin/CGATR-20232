#pragma once
#include <string>
#include "Mesh.h"

using namespace std;

class ObjReader {
public:
	ObjReader();
	~ObjReader();
	Mesh* read(string content);
	string readObjFile(string path);
	void loadTexture(const char* file_name, GLuint* tex);
};