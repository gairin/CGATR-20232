#pragma once
#include <string>
#include "Mesh.h"

using namespace std;

class ObjReader {
public:
	ObjReader();
	~ObjReader();
	Mesh* read(string content);
	void loadTexture(string path);
};