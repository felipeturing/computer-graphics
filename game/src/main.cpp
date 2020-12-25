#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ImportedModel.h"
#include "utils.h"
using namespace std;
#define numVAOs 1
#define numVBOs 6

float cameraX, cameraY, cameraZ, CottageLocX, CottageLocY, CottageLocZ,HouseLocX,HouseLocY,HouseLocZ, aspect, closeto = 0.005f;
GLuint renderingProgram, vao[numVAOs], vbo[numVBOs], mvLoc, projLoc, obj, rockTexture, houseTexture;
int width, height;
glm::mat4 pMat, vMat, mMat, mvMat;

ImportedModel Cottage("../models/Rock_big_single_b_LOD0.obj");
ImportedModel House("../models/house.obj");

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }
void setupVertices(void) {
    glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);
    // Rocks
 	std::vector<glm::vec3> vertCottage = Cottage.getVertices();
    std::vector<glm::vec2> texCottage = Cottage.getTextureCoords();
	std::vector<glm::vec3> normCottage = Cottage.getNormals();
    std::vector<float> pvaluesCottage;
    std::vector<float> tvaluesCottage;
	std::vector<float> nvaluesCottage;
    for (int i = 0; i < Cottage.getNumVertices(); i++) {
		pvaluesCottage.push_back((vertCottage[i]).x);
		pvaluesCottage.push_back((vertCottage[i]).y);
		pvaluesCottage.push_back((vertCottage[i]).z);
        tvaluesCottage.push_back((texCottage[i]).s);
		tvaluesCottage.push_back((texCottage[i]).t);
		nvaluesCottage.push_back((normCottage[i]).x);
		nvaluesCottage.push_back((normCottage[i]).y);
		nvaluesCottage.push_back((normCottage[i]).z);
	}
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvaluesCottage.size() * 4, &pvaluesCottage[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tvaluesCottage.size()*4, &tvaluesCottage[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nvaluesCottage.size() * 4, &nvaluesCottage[0], GL_STATIC_DRAW);

    //House
    std::vector<glm::vec3> vertHouse = House.getVertices();
    std::vector<glm::vec2> texHouse = House.getTextureCoords();
	std::vector<glm::vec3> normHouse = House.getNormals();
    std::vector<float> pvaluesHouse;
    std::vector<float> tvaluesHouse;
	std::vector<float> nvaluesHouse;
    for (int i = 0; i < House.getNumVertices(); i++) {
		pvaluesHouse.push_back((vertHouse[i]).x);
		pvaluesHouse.push_back((vertHouse[i]).y);
		pvaluesHouse.push_back((vertHouse[i]).z);
        tvaluesHouse.push_back((texHouse[i]).s);
		tvaluesHouse.push_back((texHouse[i]).t);
		nvaluesHouse.push_back((normHouse[i]).x);
		nvaluesHouse.push_back((normHouse[i]).y);
		nvaluesHouse.push_back((normHouse[i]).z);
	}
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, pvaluesHouse.size() * 4, &pvaluesHouse[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, tvaluesHouse.size()*4, &tvaluesHouse[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, nvaluesHouse.size() * 4, &nvaluesHouse[0], GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
    renderingProgram = createShaderProgram("shaders/vs.glsl", "shaders/fs.glsl");
//	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 200.0f; //posicion inicial viendo frontalmente la escena
    cameraX = 0.0f; cameraY = 100.0f; cameraZ = 200.0f;
	CottageLocX = -50.0f; CottageLocY = -20.0f; CottageLocZ = 0.0f;
    HouseLocX = 50.0f; HouseLocY = 0.0f; HouseLocZ = 0.0f;
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 10000.0f);
    pMat = glm::rotate(pMat, 0.5f ,glm::vec3(1.0,0.0,0.0));
	setupVertices();
    loadTexture("../textures/Rock_big_single_b_diffuse_desert.jpg", rockTexture);
    loadTexture("../textures/house/house_diffuse.jpg", houseTexture);
}
void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    obj = glGetUniformLocation(renderingProgram, "obj");
    closeto += 0.005;
    cameraX = 0.0f; cameraY = 100.0/((float)(closeto*0.01+1)); cameraZ = 500.0/((float)(closeto+1));

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(CottageLocX, CottageLocY, CottageLocZ));
    mMat = glm::scale(mMat,glm::vec3(0.5,0.5,0.5));
	mvMat = vMat * mMat;
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniform1i(obj,1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rockTexture);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArraysInstanced(GL_TRIANGLES, 0, Cottage.getNumVertices(), 10);

    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(HouseLocX, HouseLocY, HouseLocZ));
    mMat = glm::rotate(mMat, 2.5f, glm::vec3(0.0,1.0,0.0));
	mvMat = vMat * mMat;
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniform1i(obj,2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, houseTexture);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, House.getNumVertices());
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
	GLFWwindow* window = glfwCreateWindow(600, 600, "Game - Felipe T.", NULL, NULL);
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
