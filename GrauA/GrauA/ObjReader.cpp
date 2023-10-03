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

            mesh->addVertices(x, y, z);
        }

        else if (token == "vn") {
            float x, y, z;
            sline >> x >> y >> z;

            mesh->addNormals(x, y, z);
        }
        
        else if (token == "vt") {
            float x, y, z;
            sline >> x >> y >> z;

            mesh->addTexCoords(x, y, z);
        }

        else if (token == "g") {
            string groupName;
            sline >> groupName;

            group->setName(groupName);
            mesh->addGroup(group);
        }

        else if (token == "usemtl") {
            string mtllib;
            sline >> mtllib;

            mesh->setMtllib(mtllib);

        }
        else if (token == "usemtl") {
            string mtlName;
            sline >> mtlName;

            mat->setMtlName(mtlName);
            group->setMaterial(mtlName);
        }

        else if (token == "f") {
            string faceData;
            while (sline >> faceData) {
                Face* face = new Face();

                stringstream ss(faceData);
                string vs, ts, ns;

                getline(ss, vs, '/');
                getline(ss, ts, '/');
                getline(ss, ns, '/');

                int v = stoi(vs) - 1;
                face->addVertice(v);

                int t = ts != "" ? stoi(ts) - 1 : -1;
                int n = ns != "" ? stoi(ns) - 1 : -1;

                if (t > 0) {
                    face->addTexCoord(t);
                }

                if (n > 0) {
                    face->addNormal(n);
                }

                group->addFaces(face);
                delete face;
            }
        }
    }

    return mesh;
}