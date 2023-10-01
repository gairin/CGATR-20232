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

Mesh* ObjReader::read(string filename) {
    Mesh* mesh = new Mesh;
    Group* group = new Group;
    Face* face = new Face;
    Material* mat = new Material;

    ifstream arq(filename);
    if (!arq) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return nullptr;
    }

    string line;
    while (getline(arq, line)) {
        stringstream sline(line);
        string temp;
        sline >> temp;

        if (temp == "v") {
            float x, y, z;
            sline >> x >> y >> z;

            mesh->addVertices(x, y, z);
        }

        else if (temp == "g") {
            string groupName;
            sline >> groupName;

            group->setName(groupName);
        }

        else if (temp == "mtl") {
            string mtlName;
            sline >> mtlName;

            mat->setMtlName(mtlName);
        }
        
        else if (temp == "f") {
            // terminar depois
            float x, y, z;
            sline >> x, y, z;

        }
    }

    return mesh;
}