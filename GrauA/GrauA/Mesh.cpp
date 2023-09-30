#include "Mesh.h"

Mesh::Mesh() {
    // Inicialize membros da classe, se necess�rio
    mtllib = "";
    min = glm::vec3(0.0f);
    max = glm::vec3(0.0f);
    // Outras inicializa��es, se necess�rio
}

Mesh::~Mesh() {
    // Fa�a a limpeza de recursos alocados, se necess�rio
    for (glm::vec3* vertex : vertices) {
        delete vertex;
    }
    vertices.clear();
}