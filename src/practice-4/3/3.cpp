#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include "utils.h"

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

int NUMI = 10000;//Numero de lineas para formar la función seno
using namespace std;

void init(GLFWwindow *window) {

	renderingProgram = createShaderProgram("vs.glsl","fs.glsl");

	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void display(GLFWwindow *window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	GLuint offsetLoc = glGetUniformLocation(renderingProgram, "numi");
	glProgramUniform1f(renderingProgram, offsetLoc, NUMI);

	glDrawArraysInstanced(GL_LINES, 0, 2, NUMI);
}

int main(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // I don't know what this does
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);     // and neither this
	GLFWwindow *window = glfwCreateWindow(600, 600, "Ejercicio 3", NULL,
			NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
