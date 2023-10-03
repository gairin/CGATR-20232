#include "Face.h"

Face::Face() {

}

Face::~Face() {

}

void Face::addVertice(int v) {
	this->vertices.push_back(v);
}

void Face::addTexCoord(int t) {
	this->texCoords.push_back(t);
}

void Face::addNormal(int n) {
	this->normals.push_back(n);
}

vector<int> Face::getVertices() {
	return this->vertices;
}

vector<int> Face::getTexCoords() {
	return this->texCoords;
}

vector<int> Face::getNormals() {
	return this->normals;
}