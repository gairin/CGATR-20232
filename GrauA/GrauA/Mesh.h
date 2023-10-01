#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

class Mesh {
private:
    std::string mtllib;
    glm::vec3 min, max;
    std::vector<glm::vec3*> vertices;

public:
    Mesh();
    ~Mesh();
    void addVertices(float x, float y, float z);
};