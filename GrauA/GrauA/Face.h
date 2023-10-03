#pragma once
#include <vector>

using namespace std;

class Face {
private:
	vector<int> vertices;
	vector<int> texCoords;
	vector<int> normals;

public:
	Face();
	~Face();
	void addVertice(int v);
	void addTexCoord(int t);
	void addNormal(int n);
	vector<int> getVertices();
	vector<int> getTexCoords();
	vector<int> getNormals();
};