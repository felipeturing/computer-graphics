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

//VAOs and VBOs
#define numVAOs 1
#define numVBOs 15

// global variables
float cameraX, cameraY, cameraZ, RockLocX, RockLocY, RockLocZ,HouseLocX,HouseLocY,HouseLocZ, FlatLocX, FlatLocY, FlatLocZ, aspect,mountainLocX,mountainLocY,mountainLocZ,houseNavLocX,houseNavLocY,houseNavLocZ,  closeto = 3.0f, angleCamera, angleCameraInc = 0.001f;
GLuint renderingProgram, vao[numVAOs], vbo[numVBOs], mvLoc, projLoc, obj, rockTexture, houseTexture, flatTexture,houseNavTexture, mountainTexture;
int width, height, keyboard, actionKeyboard;
glm::mat4 pMat, vMat, mMat, mvMat;


// models
ImportedModel Rock("../models/Rock_big_single_b_LOD0.obj");
ImportedModel House("../models/house.obj");
ImportedModel Mountain("../models/mountain/Mountain.obj");
//ImportedModel HouseNav("../models/house-navigation/3d-model.obj");

// auxiliars and callbacks function headers
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void windowSizeCallback(GLFWwindow* win, int newWidth, int newHeight);
float toRadians(float degrees);
void setupVertices(void);
void sceneViewNavigation(void);


void init(GLFWwindow* window) {
    renderingProgram = createShaderProgram("shaders/vs.glsl", "shaders/fs.glsl");
//	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 200.0f; //posicion inicial viendo frontalmente la escena
    cameraX = 0.0f; cameraY = 20.0f; cameraZ = 500.0f;
    angleCamera = 0.5f;
	RockLocX = -200.0f; RockLocY = 0.5f; RockLocZ = 0.0f;
    HouseLocX = 50.0f; HouseLocY = 65.0f; HouseLocZ = 0.0f;
    FlatLocX = 0.0f; FlatLocY = 0.0f; FlatLocZ = 0.0f;
    mountainLocX = 0.0f; mountainLocY = 0.0f; mountainLocZ = 0.0f;
    //houseNavLocX = 50.0f; houseNavLocY = 0.0f, houseNavLocZ = 50.0f;
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 10000.0f);
    pMat = glm::rotate(pMat, angleCamera ,glm::vec3(1.0,0.0,0.0));
	setupVertices();

    loadTexture("../textures/Rock_big_single_b_diffuse_desert.jpg", rockTexture);
    loadTexture("../textures/house/house_diffuse.jpg", houseTexture);
    loadTexture("../textures/Rock_big_single_b_sandstone_flat.jpg", flatTexture);
    //loadTexture("../textures/Rock_big_single_b_sandstone_flat.jpg", flatTexture);
    //loadTexture("../textures/mountain/Color.png", mountainTexture);

}
void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    obj = glGetUniformLocation(renderingProgram, "obj");

    sceneViewNavigation();

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

    /*mMat = glm::translate(glm::mat4(1.0f), glm::vec3(mountainLocX, mountainLocY, mountainLocZ));
    mMat = glm::scale(mMat,glm::vec3(10.0,10.0,10.0));
	mvMat = vMat * mMat;
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniform1i(obj,4);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mountainTexture);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, Mountain.getNumVertices());*/

    //rocks
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(RockLocX, RockLocY, RockLocZ));
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
	glDrawArraysInstanced(GL_TRIANGLES, 0, Rock.getNumVertices(), 10);


    //House
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(HouseLocX, HouseLocY, HouseLocZ));
    mMat = glm::rotate(mMat, 2.5f, glm::vec3(0.0,1.0,0.0));
    mMat = glm::scale(mMat,glm::vec3(2.0,2.0,2.0));
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
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, House.getNumVertices());


    //Flat
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(FlatLocX, FlatLocY, FlatLocZ));
	mvMat = vMat * mMat;
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniform1i(obj,3);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, flatTexture);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, House.getNumVertices());

}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow *window;
	window = glfwCreateWindow(1200, 700, "Game", NULL, NULL);

    //set keyboard callback
    glfwSetKeyCallback(window, keyCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	glfwSetWindowSizeCallback(window, windowSizeCallback);
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

void sceneViewNavigation(void){
    if (keyboard == 87 && (actionKeyboard==GLFW_PRESS || actionKeyboard == GLFW_REPEAT) ){ //W
        cameraZ -= closeto;
    }else if (keyboard == 83 && (actionKeyboard==GLFW_PRESS || actionKeyboard == GLFW_REPEAT)){ //S
        cameraZ += closeto;
    }else if (keyboard == 65 && (actionKeyboard==GLFW_PRESS || actionKeyboard == GLFW_REPEAT)){ //S
        cameraX -= closeto;
    }else if (keyboard == 68 && (actionKeyboard==GLFW_PRESS || actionKeyboard == GLFW_REPEAT)){ //S
        cameraX += closeto;
    }else if (keyboard == 265 && (actionKeyboard==GLFW_PRESS || actionKeyboard == GLFW_REPEAT)){ //UP
        //angleCamera += angleCameraInc;
        cameraY += closeto;
    }else if (keyboard == 264 && (actionKeyboard==GLFW_PRESS || actionKeyboard == GLFW_REPEAT)){ //DOWN
        //angleCamera -= angleCameraInc;
        cameraY -= closeto;
    }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
    /**if(key ==GLFW_KEY_SPACE){
        switch (action){
            case GLFW_PRESS:
                std::cout<< "Space key is pressed" << std::endl;
                break;
            case GLFW_REPEAT:
                std::cout<<"Space key is being held down"<< std::endl;
                break;
            case GLFW_RELEASE:
                std::cout<<"Space key has been released"<<std::endl;
                break;
            default:
                break;
        }
    }**/
    keyboard = key;
    actionKeyboard = action;
//    std::cout<<key<<std::endl;
}

float toRadians(float degrees) {
    return (degrees * 2.0f * 3.14159f) / 360.0f;
}

void windowSizeCallback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

void setupVertices(void) {
    glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);
    // Rocks
 	std::vector<glm::vec3> vertRock = Rock.getVertices();
    std::vector<glm::vec2> texRock = Rock.getTextureCoords();
	std::vector<glm::vec3> normRock = Rock.getNormals();
    std::vector<float> pvaluesRock;
    std::vector<float> tvaluesRock;
	std::vector<float> nvaluesRock;
    for (int i = 0; i < Rock.getNumVertices(); i++) {
		pvaluesRock.push_back((vertRock[i]).x);
		pvaluesRock.push_back((vertRock[i]).y);
		pvaluesRock.push_back((vertRock[i]).z);
        tvaluesRock.push_back((texRock[i]).s);
		tvaluesRock.push_back((texRock[i]).t);
		nvaluesRock.push_back((normRock[i]).x);
		nvaluesRock.push_back((normRock[i]).y);
		nvaluesRock.push_back((normRock[i]).z);
	}
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvaluesRock.size() * 4, &pvaluesRock[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tvaluesRock.size()*4, &tvaluesRock[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nvaluesRock.size() * 4, &nvaluesRock[0], GL_STATIC_DRAW);

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

    //Mountain lee 1 millón de lineas del obj para la montaña, así que se demora al cargar en mi pc
    /*std::vector<glm::vec3> vertMountain = Mountain.getVertices();
    std::vector<glm::vec2> texMountain = Mountain.getTextureCoords();
	std::vector<glm::vec3> normMountain = Mountain.getNormals();
    std::vector<float> pvaluesMountain;
    std::vector<float> tvaluesMountain;
	std::vector<float> nvaluesMountain;
    for (int i = 0; i < Mountain.getNumVertices(); i++) {
		pvaluesMountain.push_back((vertMountain[i]).x);
		pvaluesMountain.push_back((vertMountain[i]).y);
		pvaluesMountain.push_back((vertMountain[i]).z);
        tvaluesMountain.push_back((texMountain[i]).s);
		tvaluesMountain.push_back((texMountain[i]).t);
		nvaluesMountain.push_back((normMountain[i]).x);
		nvaluesMountain.push_back((normMountain[i]).y);
		nvaluesMountain.push_back((normMountain[i]).z);
	}
    glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glBufferData(GL_ARRAY_BUFFER, pvaluesMountain.size() * 4, &pvaluesMountain[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glBufferData(GL_ARRAY_BUFFER, tvaluesMountain.size()*4, &tvaluesMountain[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);
	glBufferData(GL_ARRAY_BUFFER, nvaluesMountain.size() * 4, &nvaluesMountain[0], GL_STATIC_DRAW);*/





    //Plane
    float side= 2000.0f,levelY=0.0f; // lado y pos en el eje y
    float posSide = (float)side/2.0f;
    float flatPositions[18] = // 2 triangles * 3 vertex * 3 floats = 18 floats
	{ -posSide, levelY, posSide, -posSide, levelY, -posSide, posSide, levelY, posSide,
      posSide, levelY, posSide, posSide, levelY, -posSide, -posSide, levelY, -posSide};

	float textureFlatCoordinates[12] =
	{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
      1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

    glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(flatPositions), flatPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureFlatCoordinates),textureFlatCoordinates, GL_STATIC_DRAW);
}
