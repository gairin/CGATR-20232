#pragma once
#include <string>

using namespace std;

class Material {
private:
	string mtlName;

public:
	Material();
	~Material();
	void setMtlName(string name);
};