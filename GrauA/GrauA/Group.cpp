#include "Group.h"

using namespace std;

Group::Group() {

}

Group::~Group() {

}

void Group::setName(string name) {
    this->name = name;
}

void Group::setMaterial(string material) {
    this->material = material;
}

void Group::addFaces(Face* face) {
    this->faces.push_back(face);
}

vector<Face*> Group::getFaces() {
    return this->faces;
}

void Group::setVertVAO(GLuint vao) {
    this->vertVAO = vao;
}

GLuint Group::getVertVAO() {
    return this->vertVAO;
}

void Group::setTexCoordVAO(GLuint vao) {
    this->texCoordVAO = vao;
}

GLuint Group::getTexCoordVAO() {
    return this->texCoordVAO;
}

void Group::setNormVAO(GLuint vao) {
    this->normVAO = vao;
}

GLuint Group::getNormVAO() {
    return this->normVAO;
}