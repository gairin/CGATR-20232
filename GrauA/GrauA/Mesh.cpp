#include "Mesh.h"

using namespace std;

Mesh::Mesh() {
    string mtllib;
    glm::vec3 min, max;
    vector<glm::vec3*> vertices;
}

Mesh::~Mesh() {

}

void Mesh::setMtllib(string mtllib) {
    this->mtllib = mtllib;
}

void Mesh::addVertices(float x, float y, float z) {
    glm::vec3 vertice = glm::vec3(x, y, z);
    this->vertices.push_back(vertice);
}

void Mesh::addTexCoords(float x, float y, float z) {
    glm::vec3 texCoord = glm::vec3(x, y, z);
    this->texCoords.push_back(texCoord);
}

void Mesh::addNormals(float x, float y, float z) {
    glm::vec3 normal = glm::vec3(x, y, z);
    this->normals.push_back(normal);
}

void Mesh::addGroup(Group* group) {
    this->groups.push_back(group);
}

vector<Group*> Mesh::getGroups() {
    return this->groups;
}

vector<glm::vec3> Mesh::getVertices() {
    return this->vertices;
}

vector<glm::vec3> Mesh::getTexCoords() {
    return this->texCoords;
}

vector<glm::vec3> Mesh::getNormals() {
    return this->normals;
}