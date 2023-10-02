#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Group.h"

class Mesh {
private:
    std::string mtllib;
    glm::vec3 min, max;
    std::vector<glm::vec3*> vertices;
    std::vector<glm::vec3*> texCoords;
    std::vector<glm::vec3*> normals;
    std::vector<Group*> groups;

public:
    Mesh();
    ~Mesh();
    void addVertices(float x, float y, float z);
    void addTexCoords(float x, float y, float z);
    void addNormals(float x, float y, float z);
    void addGroup(Group* group);
};