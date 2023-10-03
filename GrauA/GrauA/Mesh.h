#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Group.h"

using namespace std;

class Mesh {
private:
    string mtllib;
    glm::vec3 min, max;
    vector<glm::vec3> vertices;
    vector<glm::vec3> texCoords;
    vector<glm::vec3> normals;
    vector<Group*> groups;

public:
    Mesh();
    ~Mesh();
    void setMtllib(string mtllib);
    void addVertices(float x, float y, float z);
    void addTexCoords(float x, float y, float z);
    void addNormals(float x, float y, float z);
    void addGroup(Group* group);
    vector<Group*> getGroups();
    vector<glm::vec3> getVertices();
    vector<glm::vec3> getTexCoords();
    vector<glm::vec3> getNormals();
};