#include "Material.h"

using namespace std;

Material::Material() {
	
}

void Material::setMtlName(string name) {
	this->mtlName = name;
}

string Material::getMtlName() {
	return this->mtlName;
}