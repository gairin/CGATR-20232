#include "Mesh.h"

Mesh::Mesh() {
    // Inicialize membros da classe, se necessário
    mtllib = "";
    min = glm::vec3(0.0f);
    max = glm::vec3(0.0f);
    // Outras inicializações, se necessário
}

Mesh::~Mesh() {
    // Faça a limpeza de recursos alocados, se necessário
    for (glm::vec3* vertex : vertices) {
        delete vertex;
    }
    vertices.clear();
}