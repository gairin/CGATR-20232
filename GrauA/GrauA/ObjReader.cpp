#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Mesh.h"
#include "Group.h"

using namespace std;

Mesh* read(string filename) {
    Mesh* mesh = new Mesh;
    Group* g_atual = new Group;

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
        }
    }

    return mesh;
}