#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "utils.h"
#include "ImportedModel.h"

using namespace std;

//VAOs and VBOs
#define numVAOs 1
#define numVBOs 20

// global variables
float cameraX, cameraY, cameraZ, RockLocX, RockLocY, RockLocZ,HouseLocX,HouseLocY,HouseLocZ, FlatLocX, FlatLocY, FlatLocZ,aspect,mountainLocX,mountainLocY,mountainLocZ,houseNavLocX,houseNavLocY,houseNavLocZ,humanPosZ,humanPosY,humanPosX, closeto,angleCamera, angleCameraInc, step, incRotLeg,rotLeg;
GLuint renderingProgram, vao[numVAOs], vbo[numVBOs], mvLoc, projLoc, obj, rockTexture, houseTexture, flatTexture,houseNavTexture, mountainTexture;
int width, height, keyboard, actionKeyboard;
glm::mat4 pMat, vMat, mMat, mvMat;
glm::vec4 posLeftLeg, posRightLeg, posTrunk, posHead;
stack<glm::mat4> mvStack;

// models
ImportedModel Rock("../models/Rock_big_single_b_LOD0.obj");
ImportedModel House("../models/house.obj");
ImportedModel Mountain("../models/mountain/Mountain.obj");
//ImportedModel HouseNav("../models/house-navigation/3d-model.obj");

// prototypes
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void windowSizeCallback(GLFWwindow* win, int newWidth, int newHeight);
float toRadians(float degrees);
void setupVertices(void);
void sceneViewNavigation(void);
void humanAnimation(void);

void init(GLFWwindow* window) {
    renderingProgram = createShaderProgram("shaders/vs.glsl", "shaders/fs.glsl");
    //	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 200.0f; //posicion inicial viendo frontalmente la escena
    cameraX = 0.0f; cameraY = 20.0f; cameraZ = 100.0f;
    angleCamera = 0.5f; angleCameraInc=0.001f;

	RockLocX = -300.0f; RockLocY = 0.5f; RockLocZ = 0.0f;
    HouseLocX = 500.0f; HouseLocY = 33.0f; HouseLocZ = 0.0f;
    FlatLocX = 0.0f; FlatLocY = 0.0f; FlatLocZ = 0.0f;
    mountainLocX = 0.0f; mountainLocY = 0.0f; mountainLocZ = 0.0f;

    step = 0.5f; closeto = 3.0f,incRotLeg=0.128f;

    //houseNavLocX = 50.0f; houseNavLocY = 0.0f, houseNavLocZ = 50.0f;
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.3472f, aspect, 0.1f, 100000.0f);
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
	glClearColor(0.0,0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    obj = glGetUniformLocation(renderingProgram, "obj");

    sceneViewNavigation();

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

    //mountain
    /*mMat = glm::translate(glm::mat4(1.0f), glm::vec3(mountainLocX, mountainLocY, mountainLocZ));
    mMat = glm::scale(mMat,glm::vec3(1000.0,1000.0,1000.0));
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
	glDrawArraysInstanced(GL_TRIANGLES, 0, Rock.getNumVertices(), 4);

    //House
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
	glDrawArrays(GL_TRIANGLES, 0, 6);

    humanAnimation();
}


//definitions
void humanAnimation(){
    //Human
    //head
    glUniform1i(obj,4);// 4 = human body

    if(humanPosZ > 500.0){
        step = -0.5f;
    }else if(humanPosZ < -500.0){
        step = 0.5f;
    }
    //step = 0.0; //sin traslado
    humanPosZ += step;
    humanPosX = 0.0;
    humanPosY = 27.0f;

    //la posición del humano es la de su cabeza
    posHead = glm::vec4(humanPosX,humanPosY,humanPosZ,1.0);
    mvStack.push(vMat);//M1
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(posHead));
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(2.0,2.0,2.0));
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    mvStack.pop();//quitamos el escalado de la cabeza

    //trunk
    posTrunk = glm::vec4(0.0,-9.0,0.0,1.0); // posicion del tronco respecto de la cabeza
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(posTrunk));
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(5.0,7.0,2.0));
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    mvStack.pop();

    if (rotLeg > M_PI/8.0f){
            incRotLeg = -0.05;
    }
    else if (rotLeg < -M_PI/8.0f){
            incRotLeg = 0.05;
    }
    rotLeg += incRotLeg;
    //left leg
    posLeftLeg = glm::vec4(2.0,-9.0,0.0,1.0);
    mvStack.push(mvStack.top());
    mvStack.top()  *= glm::rotate(glm::mat4(1.0f), rotLeg ,glm::vec3(1.0,0.0,0.0));
    mvStack.top()  *= glm::translate(glm::mat4(1.0f), glm::vec3(posLeftLeg));
    mvStack.top()  *= glm::scale(glm::mat4(1.0f), glm::vec3(1.5,9.0,1.5));

    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //mvStack.pop();
    mvStack.pop();

    //right leg
    posRightLeg = glm::vec4(-2.0,-9.0,0.0,1.0);
    mvStack.push(mvStack.top());
    mvStack.top()  *= glm::rotate(glm::mat4(1.0f), -rotLeg ,glm::vec3(1.0,0.0,0.0));
    mvStack.top()  *= glm::translate(glm::mat4(1.0f), glm::vec3(posRightLeg));
    mvStack.top()  *= glm::scale(glm::mat4(1.0f), glm::vec3(1.5,9.0,1.5));
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniform1i(obj,4);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    mvStack.top();
    mvStack.top();
    mvStack.top();
    mvStack.top();
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
    float flatPositions[18] = // ((6vertex)2 triangles)* 3 floats = 18 floats
	{ -posSide, levelY, posSide, -posSide, levelY, -posSide, posSide, levelY, posSide,
      posSide, levelY, posSide, posSide, levelY, -posSide, -posSide, levelY, -posSide};

	float textureFlatCoordinates[12] = //4vertex
	{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
      1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

    glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(flatPositions), flatPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureFlatCoordinates),textureFlatCoordinates, GL_STATIC_DRAW);

    //Human
    float cubePositions[108] = //36 vertex, every face using two triangles, 2x6 = 12 triangles x 3 vertex
			{ -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
					-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f,
					-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f,
					-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f,
					-1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
					-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
					-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f,
					-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f,
					-1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
					-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
					-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, };

    glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions), cubePositions, GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, vbo[12]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(textureFlatCoordinates),textureFlatCoordinates, GL_STATIC_DRAW);
}
