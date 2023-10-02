#include "Face.h"

Face::Face() {

}

Face::~Face() {

}

void Face::addVertice(int v) {
	this->vertice.push_back(v);
}

void Face::addTexCoord(int t) {
	this->texCoord.push_back(t);
}

void Face::addNormal(int n) {
	this->normal.push_back(n);
}