#include "Mesh.h"

using namespace std;

Mesh::Mesh() {
    string mtllib;
    glm::vec3 min, max = glm::vec3(0);
    vector<glm::vec3*> vertices;
}

Mesh::~Mesh() {

}

void Mesh::setMtlLib(string s) {
    s == "" ? this->mtllib = nullptr : this->mtllib = s;
}