#pragma once
#include <string>

class ObjReader {
public:
	ObjReader();
	~ObjReader();
	void read(std::string filename);
};