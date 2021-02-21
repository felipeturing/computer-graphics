#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Utils.h"
#include "Sphere.h"

using namespace std;

#define numVAOs 1
#define numVBOs 60


float cameraX, cameraY, cameraZ, aspect, angleCamera, angleCameraInc;
GLuint renderingProgram, renderingProgram1, vao[numVAOs], vbo[numVBOs], mvLoc, projLoc, nLoc, sLoc, obj;
int width, height, keyboard, closeto,actionKeyboard;

glm::mat4 pMat, vMat, mMat, mvMat,invTrMat;
glm::vec3 lightLoc(-50.0f, 50.0f, 0.0f);

//float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float globalAmbient[4] = {0.8313725f, 0.9215686f, 1.0f,1.0f};// Cool White Fluorescent
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

/*Material de oro*/
float* gMatAmb = Utils::goldAmbient();
float* gMatDif = Utils::goldDiffuse();
float* gMatSpe = Utils::goldSpecular();
float gMatShi = Utils::goldShininess();

/* material de bronce*/
float* bMatAmb = Utils::bronzeAmbient();
float* bMatDif = Utils::bronzeDiffuse();
float* bMatSpe = Utils::bronzeSpecular();
float bMatShi = Utils::bronzeShininess();

/* material de silver*/
float* sMatAmb = Utils::silverAmbient();
float* sMatDif = Utils::silverDiffuse();
float* sMatSpe = Utils::silverSpecular();
float sMatShi = Utils::silverShininess();

float thisAmb[4], thisDif[4], thisSpe[4], matAmb[4], matDif[4], matSpe[4],thisShi, matShi;

// shadow stuff
int scSizeX, scSizeY;
GLuint shadowTex, shadowBuffer;
glm::mat4 lightVmatrix,lightPmatrix,shadowMVP1,shadowMVP2,b;

// variable allocation for display
glm::vec3 currentLightPos, transformed;
float lightPos[3];
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mambLoc, mdiffLoc, mspecLoc, mshiLoc, directionLoc, outerCutOffLoc, cutOffLoc, linearLoc, quadraticLoc, constantLoc;
glm::vec3 origin(0.0f,0.0f,0.0f);
glm::vec3 up(0.0f, 10.0f, 0.0f);

//float cutOff = glm::cos(glm::radians(12.5f)), outerCutOff=glm::cos(glm::radians(17.5f)), constant = 1.0f, linear = 0.09f, quadratic = 0.032f ;
float cutOff = glm::cos(glm::radians(12.5f)), outerCutOff=glm::cos(glm::radians(17.5f)), constant = 1.0f, linear = 0.09f, quadratic = 0.032f ;
float direction[3] = {1.0f,-1.0f,1.0f};

/* Objetos y geometrias*/
Sphere mySphere = Sphere(48);

float toRadians(float degrees) {
    return (degrees * 2.0f * 3.14159f) / 360.0f;
}

void setupVertices(void) {
    glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

    /* Model : Flat
    *  Vertices
    *  Textures
    *  Normals
    */
    float side= 500.0f,levelY=0.0f; // lado y pos en el eje y
    float posSide = (float)side/2.0f;
    float flatPositions[18] = // ((6vertex)2 triangles)* 3 floats = 18 floats
	{ -posSide, levelY, posSide, -posSide, levelY, -posSide, posSide, levelY, posSide,
      posSide, levelY, posSide, posSide, levelY, -posSide, -posSide, levelY, -posSide};

    float normalsCoordinates[18] ={
        0.0,1.0,0.0,0.0,1.0,0.0,0.0,1.0,0.0,
        0.0,1.0,0.0,0.0,1.0,0.0,0.0,1.0,0.0};

    glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(flatPositions), flatPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normalsCoordinates),normalsCoordinates, GL_STATIC_DRAW);


    /* Model : Sphere
    *  Vertices
    *  Textures
    *  Normals
    */
    std::vector<int> ind = mySphere.getIndices();
	std::vector<glm::vec3> vert = mySphere.getVertices();
	std::vector<glm::vec2> tex = mySphere.getTexCoords();
	std::vector<glm::vec3> norm = mySphere.getNormals();
    std::vector<glm::vec3> tangents = mySphere.getTangents();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;
    std::vector<float> tangentsvalues;

	int numIndices = mySphere.getNumIndices();
	for (int i = 0; i < numIndices; i++) {
		pvalues.push_back((vert[ind[i]]).x);
		pvalues.push_back((vert[ind[i]]).y);
		pvalues.push_back((vert[ind[i]]).z);
		tvalues.push_back((tex[ind[i]]).s);
		tvalues.push_back((tex[ind[i]]).t);
		nvalues.push_back((norm[ind[i]]).x);
		nvalues.push_back((norm[ind[i]]).y);
		nvalues.push_back((norm[ind[i]]).z);
        tangentsvalues.push_back((tangents[ind[i]]).x);
		tangentsvalues.push_back((tangents[ind[i]]).y);
		tangentsvalues.push_back((tangents[ind[i]]).z);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo[15]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size()*4, &pvalues[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[17]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size()*4, &nvalues[0], GL_STATIC_DRAW);

}
void sceneViewNavigation(void){
    if (keyboard == 262 && (actionKeyboard==GLFW_PRESS || actionKeyboard == GLFW_REPEAT)){ //S
        cameraX -= closeto;
    }else if (keyboard == 263 && (actionKeyboard==GLFW_PRESS || actionKeyboard == GLFW_REPEAT)){ //S
        cameraX += closeto;
    }else if (keyboard == 265 && (actionKeyboard==GLFW_PRESS || actionKeyboard == GLFW_REPEAT)){ //UP
        cameraY += closeto;
    }else if (keyboard == 264 && (actionKeyboard==GLFW_PRESS || actionKeyboard == GLFW_REPEAT)){ //DOWN
        cameraY -= closeto;
    }
}
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
    keyboard = key;
    actionKeyboard = action;
}

void windowSizeCallback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}
void installLights(int renderingProgram, glm::mat4 vMatrix) {
	transformed = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
	lightPos[0] = transformed.x;
	lightPos[1] = transformed.y;
	lightPos[2] = transformed.z;

	matAmb[0] = thisAmb[0]; matAmb[1] = thisAmb[1]; matAmb[2] = thisAmb[2]; matAmb[3] = thisAmb[3];
	matDif[0] = thisDif[0]; matDif[1] = thisDif[1]; matDif[2] = thisDif[2]; matDif[3] = thisDif[3];
	matSpe[0] = thisSpe[0]; matSpe[1] = thisSpe[1]; matSpe[2] = thisSpe[2]; matSpe[3] = thisSpe[3];
	matShi = thisShi;

	// get the locations of the light and material fields in the shader
	globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
	ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
	diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
	specLoc = glGetUniformLocation(renderingProgram, "light.specular");
	posLoc = glGetUniformLocation(renderingProgram, "light.position");
    directionLoc = glGetUniformLocation(renderingProgram, "light.direction");
    cutOffLoc =  glGetUniformLocation(renderingProgram, "light.cutOff");
    outerCutOffLoc = glGetUniformLocation(renderingProgram, "light.outerCutOff");
    constantLoc = glGetUniformLocation(renderingProgram, "light.constant");
    linearLoc = glGetUniformLocation(renderingProgram, "light.linear");
    quadraticLoc  = glGetUniformLocation(renderingProgram, "light.quadratic");
	mambLoc = glGetUniformLocation(renderingProgram, "material.ambient");
	mdiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
	mspecLoc = glGetUniformLocation(renderingProgram, "material.specular");
	mshiLoc = glGetUniformLocation(renderingProgram, "material.shininess");


	// set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient);
	glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular);
	glProgramUniform3fv(renderingProgram, posLoc, 1, lightPos);
    glProgramUniform3fv(renderingProgram, directionLoc, 1, direction);
	glProgramUniform4fv(renderingProgram, mambLoc, 1, matAmb);
	glProgramUniform4fv(renderingProgram, mdiffLoc, 1, matDif);
	glProgramUniform4fv(renderingProgram, mspecLoc, 1, matSpe);
	glProgramUniform1f(renderingProgram, mshiLoc, matShi);
    glProgramUniform1f(renderingProgram, cutOff, cutOff);
    glProgramUniform1f(renderingProgram, outerCutOff, outerCutOff);
    glProgramUniform1f(renderingProgram, constant, constant);
    glProgramUniform1f(renderingProgram, linear, linear);
    glProgramUniform1f(renderingProgram, quadratic, quadratic);

}


void setupShadowBuffers(GLFWwindow* window) {
	glfwGetFramebufferSize(window, &width, &height);
	scSizeX = width;
	scSizeY = height;

	glGenFramebuffers(1, &shadowBuffer);
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


void init(GLFWwindow* window) {
    renderingProgram = Utils::createShaderProgram("shaders/vs.glsl","shaders/fs.glsl" );
    renderingProgram1 = Utils::createShaderProgram("shaders/shadow-vs.glsl","shaders/shadow-fs.glsl" );
    cameraX = 0.0f; cameraY =-50.0f; cameraZ = -100.0f;
    angleCamera = 0.5f; angleCameraInc=0.001f;
    closeto = 5.0f;
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.3472f, aspect, 0.1f, 1000000.0f);
    pMat = glm::rotate(pMat, angleCamera ,glm::vec3(1.0,0.0,0.0));

    setupVertices();
    setupShadowBuffers(window);

    b = glm::mat4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);


}

void passOne(void){
    glUseProgram(renderingProgram1);
    sLoc = glGetUniformLocation(renderingProgram1, "shadowMVP");
    mMat = glm::translate( glm::mat4(1.0f),glm::vec3(0.0,10.0,0.0));
    mMat = glm::scale(mMat,glm::vec3(10.0,10.0,10.0));
    shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[15]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    //glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
    glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

}


void passTwo(void){
    glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    obj = glGetUniformLocation(renderingProgram, "obj");
    nLoc = glGetUniformLocation(renderingProgram, "norm_matrix");
	sLoc = glGetUniformLocation(renderingProgram, "shadowMVP");

    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(cameraX, cameraY, cameraZ));
    sceneViewNavigation();

    //dibujar el plano
    thisAmb[0] = sMatAmb[0]; thisAmb[1] = sMatAmb[1]; thisAmb[2] = sMatAmb[2];
	thisDif[0] = sMatDif[0]; thisDif[1] = sMatDif[1]; thisDif[2] = sMatDif[2];
	thisSpe[0] = sMatSpe[0]; thisSpe[1] = sMatSpe[1]; thisSpe[2] = sMatSpe[2];
	thisShi = sMatShi;
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
    currentLightPos = glm::vec3(lightLoc);
	installLights(renderingProgram, vMat);
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(glm::inverse(mvMat));
	shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;
    glUniform1i(obj,0);
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);//vbo[6] : points
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);//vbo[8]: normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
    //glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
    glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Sphere
    thisAmb[0] = sMatAmb[0]; thisAmb[1] = sMatAmb[1]; thisAmb[2] = sMatAmb[2];
	thisDif[0] = sMatDif[0]; thisDif[1] = sMatDif[1]; thisDif[2] = sMatDif[2];
	thisSpe[0] = sMatSpe[0]; thisSpe[1] = sMatSpe[1]; thisSpe[2] = sMatSpe[2];
	thisShi = sMatShi;
    mMat = glm::translate(glm::mat4 (1.0f),glm::vec3(0.0,10.0,0.0));
    mMat = glm::scale(mMat,glm::vec3(10.0,10.0,10.0));
    currentLightPos = glm::vec3(lightLoc);
	installLights(renderingProgram, vMat);
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(glm::inverse(mvMat));
	shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;
    glUniform1i(obj,1);
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));
    glBindBuffer( GL_ARRAY_BUFFER , vbo [15]) ;//vertex
    glVertexAttribPointer (0 , 3 , GL_FLOAT , GL_FALSE , 0, 0) ;
    glEnableVertexAttribArray(0) ;
    glBindBuffer( GL_ARRAY_BUFFER , vbo [17]) ;//normals
    glVertexAttribPointer (1 , 3 , GL_FLOAT , GL_FALSE , 0, 0) ;
    glEnableVertexAttribArray(1) ;
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glDrawArrays( GL_TRIANGLES , 0, mySphere.getNumIndices () );

}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	currentLightPos = glm::vec3(lightLoc);
	lightVmatrix = glm::lookAt(currentLightPos, origin, up);
	lightPmatrix = glm::perspective(toRadians(60.0f), aspect, 0.1f, 1000.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.0f, 4.0f);

    passOne();
	glDisable(GL_POLYGON_OFFSET_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glDrawBuffer(GL_FRONT);
	passTwo();
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow *window;
	window = glfwCreateWindow(1200, 700, "Problema 1", NULL, NULL);

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
