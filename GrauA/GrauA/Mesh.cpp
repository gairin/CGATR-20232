#include "Mesh.h"

Mesh::Mesh() {
    std::string mtllib;
    glm::vec3 min, max;
    std::vector<glm::vec3*> vertices;
}

Mesh::~Mesh() {
    for (glm::vec3* vertex : vertices) {
        delete vertex;
    }
    vertices.clear();
}

void Mesh::addVertices(float x, float y, float z) {
    this->vertices.push_back(new glm::vec3(x, y, z));
}