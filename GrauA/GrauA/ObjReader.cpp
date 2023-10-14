#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "ObjReader.h"
#include "Mesh.h"
#include "Group.h"
#include "Face.h"
#include "Material.h"

using namespace std;

ObjReader::ObjReader() { }

ObjReader::~ObjReader() { }

Mesh* ObjReader::read(string content) {
    Mesh* mesh = new Mesh; // malha do objeto
    Group* group = new Group(); // grupo atual
    bool firstGroup = true;
    Material* mat = new Material(); // material atual

    stringstream data(content);

    string line;
    while (getline(data, line)) {
        stringstream sline(line);
        string token;
        sline >> token;

        if (token == "v") {
            float x, y, z;
            sline >> x >> y >> z;

            glm::vec3 v = glm::vec3(x, y, z);
            mesh->vertices.push_back(v);
        }

        else if (token == "vn") {
            float x, y, z;
            sline >> x >> y >> z;

            glm::vec3 vn = glm::vec3(x, y, z);
            mesh->normals.push_back(vn);
        }
        
        else if (token == "vt") {
            float x, y, z;
            sline >> x >> y >> z;

            glm::vec3 vt = glm::vec3(x, y, z);
            mesh->texCoords.push_back(vt);
        }

        else if (token == "g") {
            if (firstGroup) {
                string groupName;
                sline >> groupName;
                group->name = groupName;

                firstGroup = false;
            }

            else {
                mesh->groups.push_back(group);

                group = new Group();

                string groupName;
                sline >> groupName;
                
                group->name = groupName;
            }
        }

        else if (token == "mtllib") {
            string mtllib;
            sline >> mtllib;

            mesh->mtllib = mtllib;
        }

        else if (token == "usemtl") {
            string mtlName;
            sline >> mtlName;

            mat->mtlName = mtlName;
            group->material = mtlName;
        }

        else if (token == "f") {
            Face* face = new Face();
            string faceData;

            while (sline >> faceData) {
                stringstream ss(faceData);
                string vs, ts, ns;

                getline(ss, vs, '/');
                getline(ss, ts, '/');
                getline(ss, ns, '/');

                int v = stoi(vs) - 1;
                face->vertices.push_back(v);

                int t = ts != "" ? stoi(ts) - 1 : -1;
                int n = ns != "" ? stoi(ns) - 1 : -1;

                if (t >= 0) {
                    face->texCoords.push_back(t);
                }

                if (n >= 0) {
                    face->normals.push_back(n);
                }
            }

            group->faces.push_back(face);
        }
    }

    mesh->groups.push_back(group);
    return mesh;
}

string ObjReader::readObjFile(string path) {
    ifstream inputFile;
    inputFile.open(path);

    if (!inputFile.is_open()) {
        return nullptr;
    }

    string content;
    string line;

    while (getline(inputFile, line)) {
        content += line + "\n";
    }

    inputFile.close();
    return content;
};

void ObjReader::loadTexture(const char* file_name, GLuint* tex) {
    
}