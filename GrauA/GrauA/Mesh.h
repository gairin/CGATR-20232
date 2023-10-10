#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Group.h"

using namespace std;

class Mesh {
private:
    

public:
    Mesh();
    ~Mesh();
    void setMtlLib(string s);
    string mtllib;
    glm::vec3 min, max;
    vector<glm::vec3> vertices;
    vector<glm::vec3> texCoords;
    vector<glm::vec3> normals;
    vector<Group*> groups;
};