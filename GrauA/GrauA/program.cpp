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

Obj3D* shoot(glm::vec3& cameraPosition, glm::vec3& cameraFront);
bool collisionCheck(glm::vec3 min, glm::vec3 max, Obj3D* collider);
void readVertices(Obj3D* obj);
string loadAssets();
string loadShotAsset(string path);

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
        //"layout(location=1) in vec3 vertexColor;"
        "uniform mat4 model;"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        //"out vec3 fragColor;"
        "void main() {"
        "   gl_Position = projection * view * model * vec4(vertexPosition, 1.0);"
        //"   fragColor = vertexColor;"
        "}";

    const char* fragment_shader =
        "#version 460\n"
        //"in vec3 fragColor;"
        "out vec4 frag_color;"
        "void main() {"
        "   frag_color = vec4(1.0);"
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

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    GLint modelLoc = glGetUniformLocation(shader_programme, "model");
    GLint viewLoc = glGetUniformLocation(shader_programme, "view");
    GLint projectionLoc = glGetUniformLocation(shader_programme, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    ObjReader objReader;
    Obj3D* currentObj = new Obj3D();

    Obj3D* shot = nullptr;
    currentObj->mesh = objReader.read(loadAssets());

    Mesh* mesh = currentObj->mesh;
    readVertices(currentObj);

    // Variáveis para controlar a câmera
    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f); // posição
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // direção
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    GLfloat cameraSpeed = 0.05f;

    GLfloat pitch = 0.0f;
    GLfloat yaw = -90.0f;
    GLfloat cameraRotationSpeed = 0.1f;

    double lastTime = glfwGetTime();

    //vector<Obj3D> shots;
    float shotLifetime = 6.0f;
    float shotSpeed = 20.0f;

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Matriz de perspectiva
        glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(currentObj->transform));
        
        // Matriz de escala
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        glm::mat4 modelMatrix = scaleMatrix * currentObj->transform;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        for (Group* g : mesh->groups) {
            glBindVertexArray(g->VAO);
            //Material* material = getMaterial(g->material());
            //glBindTexture(GL_TEXTURE_2D, material->tid);
            glDrawArrays(GL_TRIANGLES, 0, g->numVertices);
        }
        
        // Configuração do tiro
        if (shot) {
            // Matriz de escala 
            glm::mat4 scaleMatrix = glm::scale(shot->transform, glm::vec3(0.02f));
            glm::mat4 modelMatrix = scaleMatrix * shot->transform;
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

            // Matriz de translação
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), shot->direction * shotSpeed * static_cast<float>(deltaTime));
            shot->transform *= translationMatrix;

            for (Group* g : shot->mesh->groups) {
                glBindVertexArray(g->VAO);
                glDrawArrays(GL_QUADS, 0, g->numVertices);
            }

            if (collisionCheck(mesh->min, mesh->max, shot)) {
                cout << "COLISÃO" << endl;
            }

            // Verificação de timeout
            if (shotLifetime >= 0) {
                shotLifetime -= deltaTime;
            }

            if (shotLifetime <= 0) {
                shot = nullptr;
            }
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
        if (!shot) {
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                shot = shoot(cameraPosition, cameraFront);
                shotLifetime = 6.0f;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
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

Obj3D* shoot(glm::vec3 &cameraPosition, glm::vec3 &cameraFront) {
    ObjReader objReader;
    Obj3D* shot = new Obj3D();

    shot->transform = glm::mat4(1);
    shot->transform[3] = glm::vec4(cameraPosition, 1.0f);
    shot->direction = cameraFront;

    shot->mesh = objReader.read(loadShotAsset("C:\\Users\\Acer\\Documents\\GitHub\\CGATR-20232\\GrauA\\Assets\\3D models\\cubo\\cube.obj"));
    shot->deletable = true;

    readVertices(shot);

    return shot;
}

bool collisionCheck(glm::vec3 min, glm::vec3 max, Obj3D* collider) {

    // Os valores sendo checados aqui não estão mudando, por isso sempre retorna true.

    if (max.x < collider->mesh->min.x || min.x > collider->mesh->max.x) {
        return false; // eixo x
    }

    if (max.y < collider->mesh->min.y || min.y > collider->mesh->max.y) {
        return false; // eixo y
    }

    if (max.z < collider->mesh->min.z || min.z > collider->mesh->max.z) {
        return false; // eixo z
    }

    return true;
}

void readVertices(Obj3D* obj) {
    Mesh* mesh = obj->mesh;

    for (Group* g : mesh->groups) {
        vector<GLfloat> vertices;
        vector<GLfloat> texCoords;
        vector<GLfloat> normals;

        for (Face* f : g->faces) {
            for (int i = 0; i < f->vertices.size(); i++) {
                glm::vec3 v = mesh->vertices[f->vertices[i]];
                vertices.push_back(v.x);
                vertices.push_back(v.y);
                vertices.push_back(v.z);
                g->numVertices++;

                mesh->min.x = glm::min(mesh->min.x, v.x);
                mesh->min.y = glm::min(mesh->min.y, v.y);
                mesh->min.z = glm::min(mesh->min.z, v.z);
                mesh->max.x = glm::max(mesh->max.x, v.x);
                mesh->max.y = glm::max(mesh->max.y, v.y);
                mesh->max.z = glm::max(mesh->max.z, v.z);

                glm::vec2 tc = mesh->texCoords[f->texCoords[i]];
                texCoords.push_back(tc.x);
                texCoords.push_back(tc.y);

                glm::vec3 n = mesh->normals[f->normals[i]];
                normals.push_back(n.x);
                normals.push_back(n.y);
                normals.push_back(n.z);
            }
        }

        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        GLuint gVertVBO, gTexCoordsVBO, gNormalsVBO;
        glGenBuffers(1, &gVertVBO);
        glBindBuffer(GL_ARRAY_BUFFER, gVertVBO);
        glBufferData(GL_ARRAY_BUFFER, (vertices.size() * sizeof(GLfloat)),
                     vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

        g->VAO = VAO;
    }
}

string loadAssets() {
    // Por enquanto, caminho absoluto para testar depois melhoro isso
    //string filePath = "C:\\Users\\Acer\\Documents\\GitHub\\CGATR-20232\\GrauA\\Assets\\3D models\\piramide\\pyramid.obj";
    //string filePath = "C:\\Users\\Acer\\Documents\\GitHub\\CGATR-20232\\GrauA\\Assets\\3D models\\mesa\\mesa\\mesa01.obj";
    //string filePath = "C:\\Users\\Acer\\Documents\\GitHub\\CGATR-20232\\GrauA\\Assets\\3D models\\trout\\trout\\trout.obj";
    //string filePath = "C:\\Users\\Acer\\Documents\\GitHub\\CGATR-20232\\GrauA\\Assets\\3D models\\cubo\\cube.obj";
    //string filePath = "C:\\Users\\Acer\\Documents\\GitHub\\CGATR-20232\\GrauA\\Assets\\3D models\\dragon\\dragon.obj";
    string filePath = "C:\\Users\\Acer\\Documents\\GitHub\\CGATR-20232\\GrauA\\Assets\\3D models\\teapot\\teapot1.obj";
    
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

// assassinando o c++ temporariamente até eu unificar a importação de assets
string loadShotAsset(string path) {
    ifstream inputFile;

    inputFile.open(path);

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