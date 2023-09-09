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

//void resize(GLFWwindow* window, int width, int height) {};
//void errorLog(int cod, const char* description) {};

// Protótipos de função
GLuint genTriangle();

int main() {
	// Inicialização da GLFW
	if (!glfwInit()) {
		return -1;
	}

	const GLuint WIDTH = 800;
	const GLuint HEIGHT = 600;

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Screensaver", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);


	//Inicialização da GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL (versão suportada) %s\n", version);


	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	//Shaders
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
	GLuint vao = genTriangle();

	// Matrizes de Transformação
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

	model *= scale;

	float speed = 0.001f;
	float angle = -1.0f;
	// x = dx; y = dy
	glm::vec2 direction = glm::vec2(cos(angle), sin(angle));

	glDeleteShader(vs);
	glDeleteShader(fs);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		
		model = glm::translate(model, glm::vec3(direction.x * speed, direction.y * speed, 0.0f));

		GLint modelLocation = glGetUniformLocation(shader_programme, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		if (model[3][0] > 0.75f || model[3][0] < -0.75f) {
			direction.x *= -1.0f;
		}
		if (model[3][1] > 0.75f || model[3][1] < -0.75f) {
			direction.y *= -1.0f;
		}

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

GLuint genTriangle() {
	GLfloat vertices[] = {
		// xyzrgb
		-0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 0.5f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.0f,  0.5f, 0.0f, 1.0f, 0.3f, 0.3f,
	};

	GLuint vbo, vao;
	
	// VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
				 vertices, GL_STATIC_DRAW);
	// Posições:
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
						  6 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);

	// Cores:
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
						  6 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	return vao;
}