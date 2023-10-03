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