#pragma once
#include <vector>

using namespace std;

class Face {
private:
	vector<int> vertice;
	vector<int> texCoord;
	vector<int> normal;

public:
	Face();
	~Face();
	void addVertice(int v);
	void addTexCoord(int t);
	void addNormal(int n);
};