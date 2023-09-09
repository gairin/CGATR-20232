// C++
#include <stdio.h>
#include <iostream>
#include <string>
#include <assert.h>

// GLEW e GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

//void resize(GLFWwindow* window, int width, int height);
//void errorLog(int cod, const char* description);

void main() {
	if (!glfwInit()) {
		
	}

	const GLuint WIDTH = 800;
	const GLuint HEIGHT = 600;

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Classe Modelagem", NULL, NULL);

	if (!window) {
		glfwTerminate();
		
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL (versão suportada) %s\n", version);


	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	const char* vertex_shader =
		"#version 460\n"
		"layout(location=0) in vec3 vp;"
		"layout(location=1) in vec3 vc;"
		"uniform mat4 model;"
		"out vec3 color;"
		"void main () {"
		"	color = vc;"
		"	gl_Position = model * vec4 (vp, 1.0);"
		"}";

	const char* fragment_shader =
		"#version 460\n"
		"in vec3 color;"
		"out vec4 frag_color;"
		"void main () {"
		"	frag_color = vec4 (color, 1.0);"
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

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}