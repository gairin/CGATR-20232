#pragma once
#include <string>
#include <vector>

#include "Face.h"

using namespace std;

class Group {
private:
	std::string name;
	std::string material;
	vector<Face*> faces;

public:
	Group();
	~Group();
	void setName(string name);
	void setMaterial(string material);
	void addFaces(Face* face);
};