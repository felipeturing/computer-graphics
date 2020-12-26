#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Cono.h"
#include "utils.h"
#include <stack>
using namespace std;
#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ, ConoX, ConoY, ConoZ, aspect;
GLuint renderingProgram, vao[numVAOs], vbo[numVBOs], mvLoc, projLoc, timeFactor, randomization;// la palabra random es reservada
stack<glm::mat4> mvStack;
int width, height,valueRandom;
glm::mat4 pMat, vMat, mMat, mvMat;
Cono myCono = Cono(48);

void setupVertices(void) {
	std::vector<int> ind = myCono.getIndices();
	std::vector<glm::vec3> vert = myCono.getVertices();
	std::vector<float> pvalues;
	int numIndices = myCono.getNumIndices();

	for (int i = 0; i < numIndices; i++) {
		pvalues.push_back((vert[ind[i]]).x);
		pvalues.push_back((vert[ind[i]]).y);
		pvalues.push_back((vert[ind[i]]).z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size()*4, &pvalues[0], GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram("vs.glsl", "fs.glsl");
	cameraX = 0.0f; cameraY = 0.0f; cameraZ =50.0f;
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    //pMat = glm::rotate(pMat, 1.57f ,glm::vec3(1.0,0.0,0.0)); // rotar la camara para una vista de la parte superior de la escena
	setupVertices();
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);

    // etiquetas unforms
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    timeFactor = glGetUniformLocation(renderingProgram, "tf");
    randomization = glGetUniformLocation(renderingProgram, "random");

    //initial compute
    srand(time(NULL));
    //valueRandom = (int)rand()%9; // 9 posibles combinaciones entres rotaciones y desplazamientos X,Y,Z
    valueRandom = (int)rand()%10;
    glUniform1i(randomization,valueRandom);


    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    mvStack.push(vMat);
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(ConoX, ConoY, ConoZ));
    mvStack.push(mvStack.top());

    // send uniforms
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniform1f(timeFactor,(float)currentTime);
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

    //
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArraysInstanced(GL_TRIANGLES, 0, myCono.getNumIndices(), 100);
    mvStack.pop();
    mvStack.pop();
    mvStack.pop();
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "Partial - p2", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	glfwSetWindowSizeCallback(window, window_size_callback);
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
