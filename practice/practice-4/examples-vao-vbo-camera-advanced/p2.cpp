#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <cmath>
#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "utils.h"

using namespace std;

#define numVAOs 1
#define numVBOs 2

// Utils util = Utils();
float cameraX, cameraY, cameraZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// variable allocation for display
GLuint mvLoc, projLoc;
// GLuint mvLoc, projLoc;
int width, height;
float aspect;
float timeFactor;
glm::mat4 pMat, vMat, mMat, mvMat;

stack<glm::mat4> mvStack;

void setupVertices(void) {
	// 12 triangles * 3 vertices * 3 values (x, y, z)
	float cubePositions[108] = { -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, };
	// 6 triangles * 3 vertices * 3 values
	float pyramidPositions[54] = { -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f,
			-1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f };

	// we need at least 1 VAO and 2 VBOs
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions), cubePositions,
			GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions,
			GL_STATIC_DRAW);
}

void init(GLFWwindow *window) {
	renderingProgram = createShaderProgram("vshader5.glsl", "fshader5.glsl");

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float) width / (float) height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians == 60 degrees

	// position the camera further down the positive Z axis (to see all of the cubes)
	cameraX = 0.0f;
	cameraY = 0.0f;
	cameraZ = 12.0f;
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

	// push view matrix onto the stack
	vMat = glm::translate(glm::mat4(1.0f),
			glm::vec3(-cameraX, -cameraY, -cameraZ));
	mvStack.push(vMat);

	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	// pyramid sun

	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f),
			glm::vec3(0.0f, 0.0f, 0.0f));  // sun position
	mvStack.push(mvStack.top());  // duplicating
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float) currentTime,
			glm::vec3(1.0f, 0.0f, 0.0f));  // sun rotation

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 18);
	mvStack.pop();

	// cube planet

	mvStack.push(mvStack.top());  // mvMat of sun
	mvStack.top() *= glm::translate(glm::mat4(1.0f),
			glm::vec3(sin((float) currentTime) * 4.0, 0.0f,
					cos((float) currentTime) * 4.0));  // planet position
	mvStack.push(mvStack.top());  // duplicating
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float) currentTime,
			glm::vec3(0.0, 1.0, 0.0));  // planet rotation

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	mvStack.pop();

	// cube moon

	mvStack.push(mvStack.top());  // mvMat of planet
	mvStack.top() *= glm::translate(glm::mat4(1.0f),
			glm::vec3(0.0f, sin((float) currentTime) * 2.0,
					cos((float) currentTime) * 2.0));  // moon position
	mvStack.push(mvStack.top());  // duplicating
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float) currentTime,
			glm::vec3(0.0, 0.0, 1.0));  // moon rotation

	mvStack.top() *= glm::scale(glm::mat4(1.0f),
			glm::vec3(0.25f, 0.25f, 0.25f));  // make the moon smaller
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	    glEnable(GL_DEPTH_TEST);
	    glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	mvStack.pop();

	mvStack.pop();
	mvStack.pop();
	mvStack.pop();
	mvStack.pop(); //Vaciamos el stack, para en el siguiente frame volver a realizar el llenado matriz por matriz
}

//void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
//    aspect = (float)newWidth / (float)newHeight;
//    glViewport(0, 0, newWidth, newHeight);
//    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
//}

int main(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // I don't know what this does
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);     // and neither this
	GLFWwindow *window = glfwCreateWindow(700,700, "Chapter4 - program3", NULL,
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
