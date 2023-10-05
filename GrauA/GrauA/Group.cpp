#include "Group.h"

using namespace std;

Group::Group() {
    this->numVertices = 0;
}

Group::~Group() {

}

void Group::setName(string name) {
    this->name = name;
}

void Group::setMaterial(string material) {
    this->material = material;
}

string Group::getMaterial() {
    return this->material;
}

void Group::addFaces(Face* face) {
    this->faces.push_back(face);
}

vector<Face*> Group::getFaces() {
    return this->faces;
}

void Group::setVAO(GLuint vao) {
    this->VAO = vao;
};

GLuint Group::getVAO() {
    return this->VAO;
};
