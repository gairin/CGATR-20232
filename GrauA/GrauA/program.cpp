// Bibliotecas do C++
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <assert.h>
#include <filesystem>
#include <vector>

// GLEW e GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

// Classes
#include "ObjReader.h";
#include "Obj3D.h";
#include "Mesh.h";

using namespace std;

string loadAssets();

struct objects {
    string name;
    string content;
};

vector<objects> assets;

// Protótipos de função

int main() {
    // Inicialização da GLFW
    if (!glfwInit()) {
        return -1;
    }

    const GLuint WIDTH = 800;
    const GLuint HEIGHT = 600;

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Grau A", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL (versão suportada) %s\n", version);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    // Shaders (colocar em arquivo separado depois se possível)
    const char* vertex_shader =
        "#version 460\n"
        "layout(location=0) in vec3 vp;"
        "layout(location=1) in vec3 vc;"
        "uniform mat4 model;"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        "out vec3 color;"
        "void main () {"
        "   color = vc;"
        "   gl_Position = projection * view * model * vec4(vp, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 460\n"
        "in vec3 color;"
        "out vec4 frag_color;"
        "void main () {"
        "   frag_color = vec4(color, 1.0);"
        "}";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    glUseProgram(shader_programme);
    glDeleteShader(vs);
    glDeleteShader(fs);

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    GLint modelLoc = glGetUniformLocation(shader_programme, "model");
    GLint viewLoc = glGetUniformLocation(shader_programme, "view");
    GLint projectionLoc = glGetUniformLocation(shader_programme, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
  
    // Cubo para testar a cena
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.5f
    };

    GLuint indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 3, 7, 7, 4, 0,
        1, 2, 6, 6, 5, 1,
        2, 3, 7, 7, 6, 2,
        0, 1, 5, 5, 4, 0
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // layout 0 = vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // layou 1 = cor
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
   
    // Ler o asset 3D
    ObjReader objReader;
    Obj3D* pyramid = new Obj3D();

    Mesh* mesh = objReader.read(loadAssets());
    pyramid->setMesh(mesh);
    // identidade placeholder
    pyramid->setTransform(glm::mat4(1));

    // Leitura dos dados
    for (Group* g : mesh->getGroups()) {
        vector<float> vertices;
        vector<float> texCoords;
        vector<float> normals;

        /* Resumir depois
        for (Face* f : g->getFaces()) {
            for (i : 0 to f->numVertices) {
                v = mesh->verts[f->verts[i]];
                vs.push_back(v.x);
                vs.push_back(v.y);
                vs.push_back(v.z);
                vt = mesh->texts[f->texts[i]];
                vts.push_back(vt.x);
                vts.push_back(vt.y);
                vn = mesh->norms[f->norms[i]];
                vns.push_back(vn.x);
                vns.push_back(vn.y);
                vns.push_back(vn.z);
            }
        }*/
    }

    // Variáveis para controlar a câmera
    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f); // posição
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // direção
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    GLfloat cameraSpeed = 0.05f;

    GLfloat pitch = 0.0f;
    GLfloat yaw = -90.0f;
    GLfloat cameraRotationSpeed = 0.1f;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        // Se possível fazer a câmera mover com o mouse
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cameraPosition += cameraSpeed * cameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cameraPosition -= cameraSpeed * cameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            pitch += cameraRotationSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            pitch -= cameraRotationSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            yaw -= cameraRotationSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            yaw += cameraRotationSpeed;
        }

        cameraFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        cameraFront.y = sin(glm::radians(pitch));
        cameraFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        cameraFront = glm::normalize(cameraFront);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    //delete mesa;
    return 0;
}

string loadAssets() {
    // Por enquanto, caminho absoluto para testar depois melhoro isso
    string filePath = "C:\\Users\\Acer\\Documents\\GitHub\\CGATR-20232\\GrauA\\Assets\\3D models\\piramide\\pyramid.obj";
    ifstream inputFile;

    inputFile.open(filePath);

    if (!inputFile.is_open()) {
        return "";
    }

    string content;
    std::string line;

    while (std::getline(inputFile, line)) {
        content += line + "\n";
    }

    inputFile.close();
    return content;
}