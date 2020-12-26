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

float cameraX, cameraY, cameraZ, treeLocX, treeLocY, treeLocZ,carLocX, carLocY, carLocZ, aspect, mov, inc = 0.001f;
GLuint renderingProgram, vao[numVAOs], vbo[numVBOs], mvLoc, projLoc, obj;
int width, height;
glm::mat4 pMat, vMat, mMat, mvMat;

ImportedModel Tree("arbol.obj");
ImportedModel Car("carro.obj");

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }
void setupVertices(void) {
    glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

    // Tree
 	std::vector<glm::vec3> vertTree = Tree.getVertices();
	std::vector<glm::vec3> normTree = Tree.getNormals();
    std::vector<float> pvaluesTree;
	std::vector<float> nvaluesTree;

    for (int i = 0; i < Tree.getNumVertices(); i++) {
		pvaluesTree.push_back((vertTree[i]).x);
		pvaluesTree.push_back((vertTree[i]).y);
		pvaluesTree.push_back((vertTree[i]).z);
		nvaluesTree.push_back((normTree[i]).x);
		nvaluesTree.push_back((normTree[i]).y);
		nvaluesTree.push_back((normTree[i]).z);
	}

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvaluesTree.size() * 4, &pvaluesTree[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, nvaluesTree.size() * 4, &nvaluesTree[0], GL_STATIC_DRAW);


    // Car
 	std::vector<glm::vec3> vertCar = Car.getVertices();
	std::vector<glm::vec3> normCar = Car.getNormals();
	std::vector<float> pvaluesCar;
	std::vector<float> nvaluesCar;

    for (int i = 0; i < Car.getNumVertices(); i++) {
		pvaluesCar.push_back((vertCar[i]).x);
		pvaluesCar.push_back((vertCar[i]).y);
		pvaluesCar.push_back((vertCar[i]).z);
		nvaluesCar.push_back((normCar[i]).x);
		nvaluesCar.push_back((normCar[i]).y);
		nvaluesCar.push_back((normCar[i]).z);
	}

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, pvaluesCar.size() * 4, &pvaluesCar[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, nvaluesCar.size() * 4, &nvaluesCar[0], GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
    renderingProgram = createShaderProgram("vs.glsl", "fs.glsl");
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 500.0f;
	treeLocX = -30.0f; treeLocY = 0.0f; treeLocZ = 500.0f;
	carLocX = 0.0f; carLocY = -100.0f; carLocZ = 0.0f;
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 10000.0f);
	setupVertices();
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.502, 0.502, 0.502, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    obj = glGetUniformLocation(renderingProgram, "obj");

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(treeLocX, treeLocY, treeLocZ));
    mMat = glm::scale(mMat, glm::vec3(100.0, 100.0, 100.0));
    mMat = glm::rotate(mMat, -1.57f, glm::vec3(1.0,0.0,0.0));

    mov += inc;
    if (mov > 1.0) mov = 0.0;

    mMat = glm::translate(mMat, glm::vec3(0.0, mov*2.0, 0.0));// lo rote así que esto traslada en Z
    mMat = glm::scale(mMat, glm::vec3(1.0/(mov*8.0+1),1.0/(mov*8.0+1),1.0/(mov*8.0+1)));
	mvMat = vMat * mMat;
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniform1i(obj, 1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); //Vincula nuestro primer VBO como el búfer activo y almacena los atributos de vértice (coordenadas)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //Especifica que nuestros datos de coordenadas van al índice de atributo 0 y contienen tres flotantes por vértice
	glEnableVertexAttribArray(0); // activa el atributo de indice 0 que se uso
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);//Especifica que nuestros datos de coordenadas de las normales van al índice de atributo 1 y contienen tres flotantes por vértice
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, Tree.getNumVertices());


    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(30.0, 0.0,500.0));
    mMat = glm::scale(mMat, glm::vec3(100.0, 100.0, 100.0));
    mMat = glm::rotate(mMat, -1.57f, glm::vec3(1.0,0.0,0.0));
    mMat = glm::translate(mMat, glm::vec3(0.0, mov*2.0, 0.0));// lo rote así que esto traslada en Z
    mMat = glm::scale(mMat, glm::vec3(1.0/(mov*8.0+1),1.0/(mov*8.0+1),1.0/(mov*8.0+1)));
	mvMat = vMat * mMat;
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniform1i(obj, 1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); //Vincula nuestro primer VBO como el búfer activo y almacena los atributos de vértice (coordenadas)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //Especifica que nuestros datos de coordenadas van al índice de atributo 0 y contienen tres flotantes por vértice
	glEnableVertexAttribArray(0); // activa el atributo de indice 0
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);//Especifica que nuestros datos de coordenadas de las normales van al índice de atributo 1 y contienen tres flotantes por vértice
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, Tree.getNumVertices());


    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(carLocX, carLocY, carLocZ));
    mvMat = vMat * mMat;
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniform1i(obj, 2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // vuelvo a usar el atributo de indice cero
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, Car.getNumVertices());
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
	GLFWwindow* window = glfwCreateWindow(600, 600, "Test 2 - p.4", NULL, NULL);
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
