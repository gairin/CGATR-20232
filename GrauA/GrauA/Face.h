#pragma once
#include <vector>

using namespace std;

class Face {
private:
	

public:
	Face();
	~Face();
	vector<int> vertices;
	vector<int> texCoords;
	vector<int> normals;
};