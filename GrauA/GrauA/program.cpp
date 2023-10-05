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
#include "ObjReader.h"
#include "Obj3D.h"
#include "Mesh.h"
#include "Material.h"

using namespace std;

string loadAssets();

vector<Obj3D> objects;

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
        "layout(location=0) in vec3 vertexPosition;"
        "layout(location=1) in vec3 vertexColor;"
        "uniform mat4 model;"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        "out vec3 fragColor;"
        "void main() {"
        "   gl_Position = projection * view * model * vec4(vertexPosition, 1.0);"
        "   fragColor = vertexColor;"
        "}";

    const char* fragment_shader =
        "#version 460\n"
        "in vec3 fragColor;"
        "out vec4 frag_color;"
        "void main() {"
        "   frag_color = vec4(fragColor, 1.0);"
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

    // Ler o asset 3D
    ObjReader objReader;
    Obj3D* pyramid = new Obj3D();

    Mesh* mesh = objReader.read(loadAssets());
    pyramid->setMesh(mesh);
    int vertNum = 0;

    for (Group* g : mesh->getGroups()) {
        vector<GLfloat> vertices;
        vector<GLfloat> texCoords;
        vector<GLfloat> normals;

        GLuint gVertVBO, gTexCoordsVBO, gNormalsVBO, VAO;
        glGenBuffers(1, &gVertVBO);
        glGenBuffers(1, &gTexCoordsVBO);
        glGenBuffers(1, &gNormalsVBO);
        glGenVertexArrays(1, &VAO);

        for (Face* f : g->getFaces()) {
            for (int i = 0; i < f->getVertices().size(); i++) {
                glm::vec3 v = mesh->getVertices()[f->getVertices()[i]];
                vertices.push_back(v.x);
                vertices.push_back(v.y);
                vertices.push_back(v.z);
                vertNum++;

                glm::vec2 tc = mesh->getTexCoords()[f->getTexCoords()[i]];
                texCoords.push_back(tc.x);
                texCoords.push_back(tc.y);

                glm::vec3 n = mesh->getNormals()[f->getNormals()[i]];
                normals.push_back(n.x);
                normals.push_back(n.y);
                normals.push_back(n.z);
            }
        }

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(2);

        glBindVertexArray(VAO);
        g->setVAO(VAO);
        glBindVertexArray(0);
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

        pyramid->setTransform(viewMatrix);

        GLint loc = glGetUniformLocation(shader_programme, "model");
        glUniformMatrix4fv(loc, 1, GL_FALSE,
                           glm::value_ptr(pyramid->getTransform()));

        for (Group* g : mesh->getGroups()) {
            
            glBindVertexArray(g->getVAO());
            //Material* material = getMaterial(g->getMaterial());
            //glBindTexture(GL_TEXTURE_2D, material->tid);
            glDrawArrays(GL_TRIANGLES, 0, g->numVertices);

            glUseProgram(0);
        }

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
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        }

        cameraFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        cameraFront.y = sin(glm::radians(pitch));
        cameraFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        cameraFront = glm::normalize(cameraFront);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
};

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
};