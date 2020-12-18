#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <string>
#include <cstdlib>
#include "utils.h"

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float triangleLocX, triangleLocY, triangleLocZ;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// variable allocation for display
GLuint vLoc, mvLoc, tfLoc, projLoc;
int width, height, displayLoopi;
float aspect;
float timeFactor;
glm::mat4 pMat, vMat, tMat, rMat, mMat, mvMat;
int NOP = 10000;
void setupVertices(void) {

    glm::vec3 myvec[NOP];
    myvec[0] = glm::vec3(-1,-1.0,0);
    myvec[1] = glm::vec3(1,-1,0);
    myvec[2] = glm::vec3(0,0.75,0);

    glm::vec3 lastpoint = myvec[0];

    srand(time(0));

    for(int i=3; i < NOP; i++) {
        lastpoint = myvec[rand()%3] + lastpoint;
        lastpoint *= 0.5;
        myvec[i] = lastpoint;
    }

	glGenVertexArrays(1, vao);  // creates VAO and returns the integer ID
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);  // creates VBO and returns the integer ID

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(myvec), myvec,
			GL_STATIC_DRAW);
}

void init(GLFWwindow *window) {
	renderingProgram = createShaderProgram("vs.glsl", "fs.glsl");

	cameraX = 0.0f;
	cameraY = 0.0f;
	cameraZ = 2.0f; //**
	triangleLocX = 0.0f;
	triangleLocY = 0.0f;
	triangleLocZ = 0.0f;
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float) width / (float) height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians == 60 degrees

    glUseProgram(renderingProgram);

	// get locations of uniforms in the shader program
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	vMat = glm::translate(glm::mat4(1.0f),
			glm::vec3(-cameraX, -cameraY, -cameraZ));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(vMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    setupVertices();
}

void display(GLFWwindow *window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);  // makes the 0th buffer "active"
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // associates 0th attribute with buffer
	glEnableVertexAttribArray(0);  // enable the 0th VERTEX ATTRIBUTE

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glPointSize(2);
	glDrawArrays(GL_POINTS, 0, NOP);
}

int main(void) {
    if (!glfwInit()) {exit(EXIT_FAILURE);}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // I don't know what this does
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // and neither this
    GLFWwindow* window = glfwCreateWindow(600, 600, "CUBO 1", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {exit(EXIT_FAILURE);}
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
