#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "utils.h"

using namespace std;

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float triangleLocX, triangleLocY, triangleLocZ;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint mvLoc, projLoc;

int width, height, displayLoopi;
float aspect;
float timeFactor;

glm::mat4 pMat, vMat, tMat, rMat, mMat, mvMat;

void setupVertices(void) {
	float triangle[9] = { -1.0f, -1.0f, 0.0f,
                        1.0f, -1.0f, 0.0f,
	                    0.0f, 0.75f, 0.0f};


	// we need at least 1 VAO and 2 VBOs
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
}

void init(GLFWwindow *window) {
	renderingProgram = createShaderProgram("vs.glsl", "fs.glsl");

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float) width / (float) height;
    //generando la matriz de proyección por perspectiva (angle, aspect, focal distance <<image plane>>, far distance)
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians == 60 degrees (abertura de la camara)
	// posicione la camara debajo del eje Z positivo, para visualizar todo el triangulo

	cameraX = 0.0f;
	cameraY = 0.0f;
	cameraZ = 4.0f;

	triangleLocX = 0.0f;
	triangleLocY = 0.0f;
	triangleLocZ = 0.0f;

	setupVertices();
}

// repeatedly
void display(GLFWwindow *window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");

	// la matriz de vista es calculada una sola para todos los objetos(en este caso solo uno) que estamos incluyendo (te lleva a las coordenadas mundo)
	vMat = glm::translate(glm::mat4(1.0f),
			glm::vec3(-cameraX, -cameraY, -cameraZ));

	// draw the triangle (use buffer #0)

	mMat = glm::translate(glm::mat4(1.0f),
			glm::vec3(triangleLocX, triangleLocY, triangleLocZ));
	mvMat = vMat * mMat;

	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat)); //generamos las variable uniform matriz de proyeccion  para enviarlas al vertex shader.
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat)); //generamos las variable uniform matriz de modelo-vista  para enviarlas al vertex shader.

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // activamos el vbo, aunque ya estaba activado
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // I don't know what this does
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);     // and neither this
	GLFWwindow *window = glfwCreateWindow(600, 600, "Ejercicio 1", NULL,
			NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);

//    glfwSetWindowSizeCallback(window, window_size_callback);

	init(window);

	// the rate at which display() is called is referred to as the frame rate
	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
