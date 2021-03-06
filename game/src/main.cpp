/*
* Autor : Walter Jesús Felipe Tolentino
* Descripción : TAU (proyecto para el curso Computación Gráfica)
*
*
*
*
* Institución : Universidad Nacional de Ingeniería - Facultad de Ciencias
*/

/** Librerias
    1.
    2.
    3.
    4.
**/


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include "Utils.h" // usando openCV para las texturas
#include "Utils2.h" // usando SOIL2 para las texturas
#include "Sphere.h"
#include "ImportedModel.h"

using namespace std;



/* Prototipos*/
/*
* passOne :
* passTwo :
* keyCallback :
*
*
*
*
*/
void passOne(void);
void passTwo(void);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void windowSizeCallback(GLFWwindow* win, int newWidth, int newHeight);
float toRadians(float degrees);
void setupVertices(void);
void sceneViewNavigation(void);
void humanBasicAnimation(void);
void sceneDessert(void);
void sceneMountain(void);
void humanAdvancedAnimation(void);
void walking(int direction);
void turning(int direction);
void jumping(void);
void humanAdvancedAnimationPassOne(void);



/*VAO y VBO*/
#define numVAOs 1
#define numVBOs 51




/** Variables globales
* *Loc(Eje)
* bool states
* positions
* materials
* stacks
*
*
* --------------------------------------------
**/
float cameraX, cameraY, cameraZ, RockLocX, RockLocY, RockLocZ, HouseLocX, HouseLocY, HouseLocZ, FlatLocX, FlatLocY, FlatLocZ, SkyLocX, SkyLocY, SkyLocZ, aspect, mountainLocX,  mountainLocY, mountainLocZ, humanPosZ, humanPosY, humanPosX, closeto, angleCamera, angleCameraInc, step, incRotLeg1, incRotLeg2, rotLeg1, rotLeg2, rotLeg3, incRotLeg3, rotArm, incRotArm, rotSky, incRotSky, rotBodyHuman, incRotBodyHuman, incJumping, deltaLight;

GLuint renderingProgram, renderingProgram1, vao[numVAOs], vbo[numVBOs], mvLoc, projLoc, nLoc, sLoc, obj,rockTexture,rockNormalMap, houseTexture,houseNormalMap, flatTexture,houseNavTexture, mountainTexture,mountainNormalMap,mountainHeightTexture, skyTexture;

int width, height, keyboard, actionKeyboard, partesDia;

bool walkingBool,jumpingBool;

glm::mat4 pMat, vMat, mMat, mvMat,invTrMat;
glm::vec4 posLeftLeg, posRightLeg, posTrunk, posHead, posLeftArm, posRightArm;
glm::vec3 lightLoc(300.0f, 500.0f, 0.0f); // Posición inicial de la Luz

stack<glm::mat4> mvStack;

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
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mambLoc, mdiffLoc, mspecLoc, mshiLoc;
glm::vec3 origin(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f, 10.0f, 0.0f);


/* Objetos y geometrias*/
ImportedModel Rock("../models/Rock_big_single_b_LOD0.obj");
ImportedModel House("../models/house.obj");
ImportedModel Mountain("../models/mountain/Mountain.obj");
Sphere mySphere = Sphere(48);


/*
* installLights
* descripción :
*
*
*
*/

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
	mambLoc = glGetUniformLocation(renderingProgram, "material.ambient");
	mdiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
	mspecLoc = glGetUniformLocation(renderingProgram, "material.specular");
	mshiLoc = glGetUniformLocation(renderingProgram, "material.shininess");

	//  set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient);
	glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular);
	glProgramUniform3fv(renderingProgram, posLoc, 1, lightPos);
	glProgramUniform4fv(renderingProgram, mambLoc, 1, matAmb);
	glProgramUniform4fv(renderingProgram, mdiffLoc, 1, matDif);
	glProgramUniform4fv(renderingProgram, mspecLoc, 1, matSpe);
	glProgramUniform1f(renderingProgram, mshiLoc, matShi);
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
    //renderingProgram = createShaderProgram("shaders/vs.glsl", "shaders/fs.glsl");
    renderingProgram = Utils::createShaderProgram("shaders/vs.glsl","shaders/fs.glsl" );
    renderingProgram1 = Utils::createShaderProgram("shaders/shadow-vs.glsl","shaders/shadow-fs.glsl" );
    cameraX = 0.0f; cameraY = 3.0f; cameraZ = 100.0f;
    angleCamera = 0.5f; angleCameraInc=0.001f;

	RockLocX = -300.0f; RockLocY = 0.5f; RockLocZ = 0.0f;
    HouseLocX = 0.0f; HouseLocY = 33.0f; HouseLocZ = -200.0f;
    FlatLocX = 0.0f; FlatLocY = 0.0f; FlatLocZ = 0.0f;
    mountainLocX = 0.0f; mountainLocY = 0.0f; mountainLocZ = 0.0f;
    SkyLocX = 0.0f;SkyLocY = 0.0f;SkyLocZ = 0.0f;
    humanPosZ = 300.0;humanPosX = 0.0;humanPosY = 40.0;

    step = 0.4f; closeto = 3.0f;rotLeg1=0.0;rotLeg2=0.0;
    incRotLeg1=0.05f/2.0f;incRotLeg2=2.0*(0.05f/2.0f);
    incRotArm=0.05f/2.0f; incRotLeg3 = 0.0; rotLeg3=0.0;
    rotBodyHuman = 0.0f; incRotBodyHuman = 2.0*(0.05f/2.0f);
    rotSky = 0.0f; incRotSky = 0.0001;incJumping = 0.5f;

    deltaLight = 0.005;
    partesDia = 1;//mañana

    jumpingBool = false;walkingBool=false;

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

    /*loadTexture("../textures/Rock_big_single_b_diffuse_desert.jpg", rockTexture);
    loadTexture("../textures/house/house_diffuse.jpg", houseTexture);
    loadTexture("../textures/Rock_big_single_b_sandstone_flat.jpg", flatTexture);
    loadTexture("../textures/mountain/Color.png", mountainTexture);
    loadTexture("../textures/sky/sky4.jpg", skyTexture);*/

    rockTexture = Utils::loadTexture("./../textures/Rock_big_single_b_diffuse_desert.tga");
    houseTexture = Utils::loadTexture("./../textures/house/house_diffuse.tga");
    flatTexture = Utils::loadTexture("./../textures/Rock_big_single_b_sandstone_flat.tga");
    skyTexture = Utils::loadTexture("./../textures/sky/sky4.jpg");
    rockNormalMap = Utils::loadTexture("./../textures/Rock_big_single_b_normals.tga");
    houseNormalMap =Utils::loadTexture("./../textures/house/house_normal.tga");
    mountainNormalMap = Utils::loadTexture("./../textures/mountain/Normal.tga");
    mountainTexture = Utils::loadTexture("./../textures/mountain/Color.png");
    mountainHeightTexture = Utils::loadTexture("./../textures/mountain/Height.jpg");
}

void passOne(void){
    glUseProgram(renderingProgram1);
    sLoc = glGetUniformLocation(renderingProgram1, "shadowMVP");
    obj = glGetUniformLocation(renderingProgram1, "obj");

    //dibujar el plano
    /*mMat = glm::translate(glm::mat4(1.0f), glm::vec3(FlatLocX, FlatLocY, FlatLocZ));
    shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));
    glUniform1i(obj,2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
    glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 6);*/

    /*mMat = glm::translate(glm::mat4(1.0f), glm::vec3(mountainLocX, mountainLocY, mountainLocZ));
    mMat = glm::scale(mMat,glm::vec3(10.0,10.0,10.0));
    shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
    glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));
    glUniform1i(obj,4);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[30]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    //glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
    glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, Mountain.getNumVertices());*/

     //dibujar las rocas
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(RockLocX, RockLocY, RockLocZ));
    shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
    glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));
    glUniform1i(obj,1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
    glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glDrawArraysInstanced(GL_TRIANGLES, 0, Rock.getNumVertices(), 4);

    //dibujar la casa
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(HouseLocX, HouseLocY+30.0f, HouseLocZ));
    mMat = glm::rotate(mMat, 2.5f, glm::vec3(0.0,1.0,0.0));
    mMat = glm::scale(mMat, glm::vec3(2.0,2.0,2.0));
    shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));
    glUniform1i(obj,2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    //glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, House.getNumVertices());


    mMat = glm::translate( glm::mat4(1.0f),glm::vec3(SkyLocX,SkyLocY,SkyLocZ));
    mMat = glm::rotate(mMat,rotSky,glm :: vec3 (0.0 ,1.0 ,0.0));
    mMat = glm::scale(mMat,glm::vec3(20000.0,20000.0,20000.0));
    shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));
    glUniform1i(obj,2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[15]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glDrawArrays( GL_TRIANGLES , 0, mySphere . getNumIndices () );

    //robot
    humanAdvancedAnimationPassOne();

}


void passTwo(void){
    glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    obj = glGetUniformLocation(renderingProgram, "obj");
    nLoc = glGetUniformLocation(renderingProgram, "norm_matrix");
	sLoc = glGetUniformLocation(renderingProgram, "shadowMVP");

    vMat = glm::lookAt(glm::vec3(cameraX, cameraY, cameraZ)+glm::vec3(humanPosX, humanPosY,humanPosZ),glm::vec3(humanPosX, humanPosY,humanPosZ),glm::vec3(0.0f, 10.0f, 0.0f));
    /*vMat = glm::translate(glm::mat4(1.0f), glm::vec3(cameraX, cameraY, cameraZ));*/

    sceneViewNavigation();

    //dibujar la montaña
    /*thisAmb[0] = sMatAmb[0]; thisAmb[1] = sMatAmb[1]; thisAmb[2] = sMatAmb[2];  // bronze
	thisDif[0] = sMatDif[0]; thisDif[1] = sMatDif[1]; thisDif[2] = sMatDif[2];
	thisSpe[0] = sMatSpe[0]; thisSpe[1] = sMatSpe[1]; thisSpe[2] = sMatSpe[2];
	thisShi = sMatShi;

    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(mountainLocX, mountainLocY, mountainLocZ));
    mMat = glm::scale(mMat,glm::vec3(10.0,10.0,10.0));
	currentLightPos = glm::vec3(lightLoc);
	installLights(renderingProgram, vMat);
	mvMat = vMat * mMat;
    invTrMat = glm::transpose(glm::inverse(mvMat));
	shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));
    glUniform1i(obj,4);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[30]);//vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[31]);//textures
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[32]);//normals
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[43]);//tangents
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mountainTexture);
    glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mountainNormalMap);
    glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mountainHeightTexture);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, Mountain.getNumVertices());*/

    //dibujar el plano
    thisAmb[0] = bMatAmb[0]; thisAmb[1] = bMatAmb[1]; thisAmb[2] = bMatAmb[2];  // bronze
	thisDif[0] = bMatDif[0]; thisDif[1] = bMatDif[1]; thisDif[2] = bMatDif[2];
	thisSpe[0] = bMatSpe[0]; thisSpe[1] = bMatSpe[1]; thisSpe[2] = bMatSpe[2];
	thisShi = bMatShi;

    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(FlatLocX, FlatLocY, FlatLocZ));

    currentLightPos = glm::vec3(lightLoc);
	installLights(renderingProgram, vMat);

	mvMat = vMat * mMat;
	invTrMat = glm::transpose(glm::inverse(mvMat));
	shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;

    glUniform1i(obj,4);
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
    glBindBuffer(GL_ARRAY_BUFFER, vbo[47]);//vbo[47]: tangents
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);//vbo[7]: textures
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
    glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, flatTexture);
    glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, rockNormalMap);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    //glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
    glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 6);


    //dibujar las rocas
    thisAmb[0] = bMatAmb[0]; thisAmb[1] = bMatAmb[1]; thisAmb[2] = bMatAmb[2];  // bronze
	thisDif[0] = bMatDif[0]; thisDif[1] = bMatDif[1]; thisDif[2] = bMatDif[2];
	thisSpe[0] = bMatSpe[0]; thisSpe[1] = bMatSpe[1]; thisSpe[2] = bMatSpe[2];
	thisShi = bMatShi;

    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(RockLocX, RockLocY, RockLocZ));
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


    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[49]);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
    glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, rockTexture);
    glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, rockNormalMap);
    //glClear(GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	glDrawArraysInstanced(GL_TRIANGLES, 0, Rock.getNumVertices(), 4);

    //dibujar la casa
    thisAmb[0] = sMatAmb[0]; thisAmb[1] = sMatAmb[1]; thisAmb[2] = sMatAmb[2];  // silver
	thisDif[0] = sMatDif[0]; thisDif[1] = sMatDif[1]; thisDif[2] = sMatDif[2];
	thisSpe[0] = sMatSpe[0]; thisSpe[1] = sMatSpe[1]; thisSpe[2] = sMatSpe[2];
	thisShi = sMatShi;

    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(HouseLocX, HouseLocY+30.0, HouseLocZ));
    mMat = glm::rotate(mMat, 2.5f, glm::vec3(0.0,1.0,0.0));
    mMat = glm::scale(mMat, glm::vec3(2.0,2.0,2.0));
    currentLightPos = glm::vec3(lightLoc);
	installLights(renderingProgram, vMat);
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(glm::inverse(mvMat));
	shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;

    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));
    glUniform1i(obj,2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);//vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);//normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[46]);//tangents
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);//textures
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, houseTexture);
    glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, houseNormalMap);
    //glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, House.getNumVertices());

    // Sky
    thisAmb[0] = bMatAmb[0]; thisAmb[1] = bMatAmb[1]; thisAmb[2] = bMatAmb[2];  // bronze
	thisDif[0] = bMatDif[0]; thisDif[1] = bMatDif[1]; thisDif[2] = bMatDif[2];
	thisSpe[0] = bMatSpe[0]; thisSpe[1] = bMatSpe[1]; thisSpe[2] = bMatSpe[2];
	thisShi = bMatShi;
    rotSky += incRotSky ;
    mMat = glm::translate( glm :: mat4 (1.0f) , glm :: vec3( SkyLocX , SkyLocY , SkyLocZ ) );
    mMat = glm::rotate( mMat , rotSky , glm :: vec3 (0.0 ,1.0 ,0.0) );
    mMat = glm::scale( mMat , glm::vec3 (10000.0 ,10000.0 ,10000.0) ) ;
    currentLightPos = glm::vec3(lightLoc);
	installLights(renderingProgram, vMat);
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(glm::inverse(mvMat));
	shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));
    glUniform1i(obj,50);
    glBindBuffer( GL_ARRAY_BUFFER , vbo [15]) ;//vertex
    glVertexAttribPointer (0 , 3 , GL_FLOAT , GL_FALSE , 0, 0) ;
    glEnableVertexAttribArray(0) ;
    glBindBuffer( GL_ARRAY_BUFFER , vbo [17]) ;//normals
    glVertexAttribPointer (1 , 3 , GL_FLOAT , GL_FALSE , 0, 0) ;
    glEnableVertexAttribArray(1) ;
    glBindBuffer( GL_ARRAY_BUFFER , vbo [50]) ;//tangents
    glVertexAttribPointer (3 , 3 , GL_FLOAT , GL_FALSE , 0, 0) ;
    glEnableVertexAttribArray(3) ;
    glBindBuffer( GL_ARRAY_BUFFER , vbo [16]) ;//textures
    glVertexAttribPointer (2 , 2 , GL_FLOAT , GL_FALSE , 0, 0) ;
    glEnableVertexAttribArray (2) ;
    glActiveTexture( GL_TEXTURE1) ;
    glBindTexture( GL_TEXTURE_2D , skyTexture );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    //glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
    glDrawArrays( GL_TRIANGLES , 0, mySphere . getNumIndices () );


    //Robot
    humanAdvancedAnimation();

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



    if(partesDia == 3){// transicion dia noche
        globalAmbient[0] -= deltaLight;
        globalAmbient[1] -= deltaLight;
        globalAmbient[2] -= deltaLight;
        lightDiffuse[0] -= deltaLight*0.1;
        lightDiffuse[1] -= deltaLight*0.1;
        lightDiffuse[2] -= deltaLight*0.1;

    }else if(partesDia == 4){//transicion noche dia
        globalAmbient[0] += deltaLight;
        globalAmbient[1] += deltaLight;
        globalAmbient[2] += deltaLight;
        lightDiffuse[0] += deltaLight*0.1;
        lightDiffuse[1] += deltaLight*0.1;
        lightDiffuse[2] += deltaLight*0.1;
    }

    //cout<<currentTime<<endl;
    if((int)currentTime % 40 == 10){
       partesDia = 3;
    }else if((int)currentTime % 40 == 20){
        partesDia = 2;
    }else if((int)currentTime % 40 == 30){
        partesDia = 4;
    }else if((int)currentTime % 40 == 0){
        partesDia = 1;//luz
    }


    if (keyboard == 84 && (actionKeyboard==GLFW_PRESS || actionKeyboard == GLFW_REPEAT)){ //T
        walking(1);
    }else if (keyboard == 71 && (actionKeyboard==GLFW_PRESS || actionKeyboard == GLFW_REPEAT)){ //G
        walking(2);
    }

    if (keyboard == GLFW_KEY_SPACE && actionKeyboard==GLFW_PRESS ){ //space
        jumpingBool = true;
    }
    if(jumpingBool == true){
        jumping();
    }

    if (keyboard == 72 && (actionKeyboard==GLFW_PRESS || actionKeyboard == GLFW_REPEAT)){ //H
        turning(1);
    }else if (keyboard == 70 && (actionKeyboard==GLFW_PRESS || actionKeyboard == GLFW_REPEAT)){ //F
        turning(2);
    }

    passOne();
	glDisable(GL_POLYGON_OFFSET_FILL);	// artifact reduction, continued
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glDrawBuffer(GL_FRONT);
	passTwo();
}


void walking(int direction){
    walkingBool=true;

    if(direction == 1){//hacia adelante
        humanPosZ -= step*cos(rotBodyHuman);
        humanPosX -= step*sin(rotBodyHuman);
    }else if(direction == 2){//hacia atras
        humanPosZ += step*cos(rotBodyHuman);
        humanPosX += step*sin(rotBodyHuman);
    }

    if (rotArm > M_PI/8.0){ // Rotar brazos desde -M_PI/8 hasta M_PI/8
            incRotArm = -0.05/2.0;
    }
    else if (rotArm < -M_PI/8.0){
        incRotArm = 0.05/2.0;
    }
    rotArm += incRotArm;


    if (rotLeg1 > M_PI/8.0){ // Rotar muslos de de -M_PI/8 hasta M_PI/8
        incRotLeg1 = -0.05/2.0;
    }
    else if (rotLeg1 < -M_PI/8.0){
        incRotLeg1 = 0.05/2.0;
    }
    rotLeg1 += incRotLeg1;


    if(rotLeg2 > M_PI/4.0f){ //Rotación de canilla derecha inicialmente está rotado M_PI/4
        incRotLeg2 = 0.0;
    }
    if(rotLeg1 < -M_PI/8.0){ //comienza cuando el muslo está en -M_PI/8
        incRotLeg2 = -2.0*(0.05/2.0);
    }
    if(rotLeg2 < 0.0){
        incRotLeg2 = 2.0*(0.05/2.0);
    }
    rotLeg2 += incRotLeg2; //


    if(rotLeg3 > M_PI/4.0f){//Rotación de canilla izquierda
        incRotLeg3 = -2.0*(0.05/2.0);
    }
    if(rotLeg1 > M_PI/8.0){ //Rotación de canilla izquierda comienza cuando el muslo está en -M_PI/8
        incRotLeg3 = 2.0*(0.05/2.0);
    }
    if(rotLeg1 < -M_PI/8.0){
        incRotLeg3 = 0.0;
    }
    rotLeg3 += incRotLeg3;
}

void turning(int direction){
    if(direction == 1){
        rotBodyHuman -= incRotBodyHuman;
    }else if(direction == 2){
        rotBodyHuman += incRotBodyHuman;
    }
}

void jumping(void){
    humanPosY += incJumping;
    if (humanPosY > 48.0){ // 8.0f de salto
        incJumping = -0.5;
    }
    else if (humanPosY == 40.0){
        jumpingBool = false;
        incJumping = 0.5;
    }
}
void humanAdvancedAnimationPassOne(void){
    //Head
    glUniform1i(obj,2);
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(humanPosX,humanPosY,humanPosZ));
    mMat = glm::rotate(mMat, (float)M_PI  ,glm::vec3(0.0,1.0,0.0));
    mMat = glm::rotate(mMat, rotBodyHuman  ,glm::vec3(0.0,1.0,0.0));
    mMat = glm::scale(mMat, glm::vec3(9.0,9.0,9.0));
    shadowMVP1 = lightPmatrix * lightVmatrix *  mMat;
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);//vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void humanAdvancedAnimation(void){

    thisAmb[0] = sMatAmb[0]; thisAmb[1] = sMatAmb[1]; thisAmb[2] = sMatAmb[2];  //silver
	thisDif[0] = sMatDif[0]; thisDif[1] = sMatDif[1]; thisDif[2] = sMatDif[2];
	thisSpe[0] = sMatSpe[0]; thisSpe[1] = sMatSpe[1]; thisSpe[2] = sMatSpe[2];
	thisShi = sMatShi;

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, houseTexture);
    glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, rockNormalMap);
    glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mountainHeightTexture);
    currentLightPos = glm::vec3(humanPosX,humanPosY-10.0,humanPosZ);
	installLights(renderingProgram, vMat);

    //Head
    mvStack.push(vMat);
    glUniform1i(obj,43);
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(humanPosX,humanPosY,humanPosZ));
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)M_PI  ,glm::vec3(0.0,1.0,0.0));//mirar hacia adelante
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), rotBodyHuman  ,glm::vec3(0.0,1.0,0.0));//rotacion
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(2.0,2.25,2.0));
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    //glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);//vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[14]);//normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[42]);//tangents
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);//vbo[7]: textures
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    mvStack.pop();

    //Neck
    glUniform1i(obj,43);
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-(2.25 + 1.0),0.0));
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0,1.0,1.0));
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    mvStack.pop();

    //Trunk top
    glUniform1i(obj,42);
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-(1.00 + 5.0),0.0));
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(4.0,5.0,2.0));
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    mvStack.pop();

    //ARMS
        // Right
                //Arm top
                glUniform1i(obj,42);
                mvStack.push(mvStack.top());
                mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(4.0 + 1.0 ,5.0-3.5+4.5,0.0));
                mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)M_PI/50.0f ,glm::vec3(0.0,0.0,1.0));
                mvStack.top() *= glm::rotate(glm::mat4(1.0f), rotArm , glm::vec3(1.0,0.0,0.0));
                mvStack.push(mvStack.top());
                mvStack.top()  *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-4.5,0.0));
                mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0,3.5,1.0));
                glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
                glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	            glEnableVertexAttribArray(0);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                mvStack.pop();

                //Forearm
                glUniform1i(obj,43);
                mvStack.push(mvStack.top());
                mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-2.5-4.5,0.0));
                mvStack.top() *= glm::rotate(glm::mat4(1.0f), -(float)M_PI/10.0f, glm::vec3(1.0,0.0,0.0));
                mvStack.push(mvStack.top());
                mvStack.top()  *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-4.0,0.0));
                mvStack.top()  *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0,3.0,1.0));
                glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
                glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	            glEnableVertexAttribArray(0);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                mvStack.pop();

                //hand
                glUniform1i(obj,42);
                mvStack.push(mvStack.top());
                mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-(7.0 + 1.5),0.0));
                mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0,1.5,1.0));
                glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
                glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	            glEnableVertexAttribArray(0);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                mvStack.pop();
                mvStack.pop();
                mvStack.pop();

        // Left
                //Arm top
                glUniform1i(obj,42);
                mvStack.push(mvStack.top());
                mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(-4.0 - 1.0,5.0-3.5+4.5,0.0));
                mvStack.top() *= glm::rotate(glm::mat4(1.0f), -(float)M_PI/50.0f ,glm::vec3(0.0,0.0,1.0));
                mvStack.top() *= glm::rotate(glm::mat4(1.0f), -rotArm , glm::vec3(1.0,0.0,0.0));
                mvStack.push(mvStack.top());
                mvStack.top()  *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-4.5,0.0));
                mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0,3.5,1.0));
                glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
                glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	            glEnableVertexAttribArray(0);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                mvStack.pop();

                //Forearm
                glUniform1i(obj,43);
                mvStack.push(mvStack.top());
                mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-2.5-4.5,0.0));
                mvStack.top() *= glm::rotate(glm::mat4(1.0f), -(float)M_PI/10.0f, glm::vec3(1.0,0.0,0.0));
                mvStack.push(mvStack.top());
                mvStack.top()  *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-4.0,0.0));
                mvStack.top()  *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0,3.0,1.0));
                glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
                glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	            glEnableVertexAttribArray(0);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                mvStack.pop();

                //hand
                glUniform1i(obj,42);
                mvStack.push(mvStack.top());
                mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-(7.0 + 1.5),0.0));
                mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0,1.5,1.0));
                glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
                glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	            glEnableVertexAttribArray(0);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                mvStack.pop();
                mvStack.pop();
                mvStack.pop();

    //Trunk down
    glUniform1i(obj,44);
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-(5.0 + 1.5),0.0));
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(4.0,1.5,2.0));
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    mvStack.pop();

    //LEGS
            //RIGHT
                glUniform1i(obj,44);
                mvStack.push(mvStack.top());
                mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(2.5,-(1.5 + 4.5) + 5.5,0.0));
                mvStack.top() *= glm::rotate(glm::mat4(1.0f), -rotLeg1 , glm::vec3(1.0,0.0,0.0));
                mvStack.push(mvStack.top());
                mvStack.top() *= glm::translate(glm::mat4(1.0f),glm::vec3(0.0,-5.5,0.0));
                mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.5,4.5,1.75));
                glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
                glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	            glEnableVertexAttribArray(0);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                mvStack.pop();

                glUniform1i(obj,43);
                mvStack.push(mvStack.top());
                mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-(4.5 + 4.5 + 5.5)+5.5,0.0));
                mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)M_PI/4.0f, glm::vec3(1.0,0.0,0.0));
                mvStack.top() *= glm::rotate(glm::mat4(1.0f), -rotLeg2, glm::vec3(1.0,0.0,0.0));
                mvStack.push(mvStack.top());
                mvStack.top() *= glm::translate(glm::mat4(1.0f),glm::vec3(0.0,-5.5,0.0));
                mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.5,4.5,1.75));
                glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
                glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	            glEnableVertexAttribArray(0);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                mvStack.pop();
                //shoes
                glUniform1i(obj,42);
                mvStack.push(mvStack.top());
                mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-(4.5+1.0+5.5),1.0));
                mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.5,1.0,3.0));
                glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
                glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	            glEnableVertexAttribArray(0);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                mvStack.pop();
                mvStack.pop();
                mvStack.pop();

            //LEFT
                glUniform1i(obj,44);
                mvStack.push(mvStack.top());
                mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(-2.5,-(1.5 + 4.5) + 5.5,0.0));
                mvStack.top() *= glm::rotate(glm::mat4(1.0f), rotLeg1 , glm::vec3(1.0,0.0,0.0));
                mvStack.push(mvStack.top());
                mvStack.top() *= glm::translate(glm::mat4(1.0f),glm::vec3(0.0,-5.5,0.0));
                mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.5,4.5,1.75));
                glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
                glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	            glEnableVertexAttribArray(0);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                mvStack.pop();

                glUniform1i(obj,43);
                mvStack.push(mvStack.top());
                mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-(4.5 + 4.5 + 5.5)+5.5,0.0));
                mvStack.top() *= glm::rotate(glm::mat4(1.0f), rotLeg3, glm::vec3(1.0,0.0,0.0));

                mvStack.push(mvStack.top());
                mvStack.top() *= glm::translate(glm::mat4(1.0f),glm::vec3(0.0,-5.5,0.0));
                mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.5,4.5,1.75));
                glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
                glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	            glEnableVertexAttribArray(0);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                mvStack.pop();
                //shoes
                glUniform1i(obj,42);
                mvStack.push(mvStack.top());
                mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-(4.5+1.0+5.5),1.0));
                mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.5,1.0,3.0));
                glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
                glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	            glEnableVertexAttribArray(0);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                mvStack.pop();

    mvStack.pop();
    mvStack.pop();
    mvStack.pop();
    mvStack.pop();
    mvStack.pop();
}


int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow *window;
	window = glfwCreateWindow(1200, 700, "TAU v0.01", NULL, NULL);

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

    /* Model : Rocks
    *  Vertices
    *  Textures
    *  Normals
    */
 	std::vector<glm::vec3> vertRock = Rock.getVertices();
    std::vector<glm::vec2> texRock = Rock.getTextureCoords();
	std::vector<glm::vec3> normRock = Rock.getNormals();
    std::vector<glm::vec3> tanRock = Rock.getTangents();
    std::vector<glm::vec3> bitRock = Rock.getBitangents();
    std::vector<float> pvaluesRock;
    std::vector<float> tvaluesRock;
	std::vector<float> nvaluesRock;
    std::vector<float> tavaluesRock;
    std::vector<float> bivaluesRock;

    for (int i = 0; i < Rock.getNumVertices(); i++) {
		pvaluesRock.push_back((vertRock[i]).x);
		pvaluesRock.push_back((vertRock[i]).y);
		pvaluesRock.push_back((vertRock[i]).z);

        tvaluesRock.push_back((texRock[i]).s);
		tvaluesRock.push_back((texRock[i]).t);

		nvaluesRock.push_back((normRock[i]).x);
		nvaluesRock.push_back((normRock[i]).y);
		nvaluesRock.push_back((normRock[i]).z);

        tavaluesRock.push_back((tanRock[i]).x);
        tavaluesRock.push_back((tanRock[i]).y);
        tavaluesRock.push_back((tanRock[i]).z);

        bivaluesRock.push_back((bitRock[i]).x);
        bivaluesRock.push_back((bitRock[i]).y);
        bivaluesRock.push_back((bitRock[i]).z);

	}
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvaluesRock.size() * 4, &pvaluesRock[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tvaluesRock.size()*4, &tvaluesRock[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nvaluesRock.size() * 4, &nvaluesRock[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[49]);
	glBufferData(GL_ARRAY_BUFFER, tavaluesRock.size() * 4, &tavaluesRock[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[48]);
	glBufferData(GL_ARRAY_BUFFER, bivaluesRock.size() * 4, &bivaluesRock[0], GL_STATIC_DRAW);



    /* Model : House
    *  Vertices
    *  Textures
    *  Normals
    */
    std::vector<glm::vec3> vertHouse = House.getVertices();
    std::vector<glm::vec2> texHouse = House.getTextureCoords();
	std::vector<glm::vec3> normHouse = House.getNormals();
    std::vector<glm::vec3> tanHouse= House.getTangents();
    std::vector<glm::vec3> bitHouse= House.getBitangents();
    std::vector<float> pvaluesHouse;
    std::vector<float> tvaluesHouse;
	std::vector<float> nvaluesHouse;
    std::vector<float> tavaluesHouse;
    std::vector<float> bivaluesHouse;

    for (int i = 0; i < House.getNumVertices(); i++) {
		pvaluesHouse.push_back((vertHouse[i]).x);
		pvaluesHouse.push_back((vertHouse[i]).y);
		pvaluesHouse.push_back((vertHouse[i]).z);
        tvaluesHouse.push_back((texHouse[i]).s);
		tvaluesHouse.push_back((texHouse[i]).t);
		nvaluesHouse.push_back((normHouse[i]).x);
		nvaluesHouse.push_back((normHouse[i]).y);
		nvaluesHouse.push_back((normHouse[i]).z);

        tavaluesHouse.push_back((tanRock[i]).x);
        tavaluesHouse.push_back((tanRock[i]).y);
        tavaluesHouse.push_back((tanRock[i]).z);

        bivaluesHouse.push_back((bitHouse[i]).x);
        bivaluesHouse.push_back((bitHouse[i]).y);
        bivaluesHouse.push_back((bitHouse[i]).z);
	}
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, pvaluesHouse.size() * 4, &pvaluesHouse[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, tvaluesHouse.size()*4, &tvaluesHouse[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, nvaluesHouse.size() * 4, &nvaluesHouse[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[46]);
	glBufferData(GL_ARRAY_BUFFER, tavaluesHouse.size() * 4, &tavaluesHouse[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[45]);
	glBufferData(GL_ARRAY_BUFFER, bivaluesHouse.size() * 4, &bivaluesHouse[0], GL_STATIC_DRAW);



    //Mountain
    std::vector<glm::vec3> vertMountain = Mountain.getVertices();
    std::vector<glm::vec2> texMountain = Mountain.getTextureCoords();
	std::vector<glm::vec3> normMountain = Mountain.getNormals();
    std::vector<glm::vec3> tanMountain= Mountain.getTangents();
    std::vector<glm::vec3> bitMountain= Mountain.getBitangents();

    std::vector<float> pvaluesMountain;
    std::vector<float> tvaluesMountain;
	std::vector<float> nvaluesMountain;
    std::vector<float> tavaluesMountain;
    std::vector<float> bivaluesMountain;

    for (int i = 0; i < Mountain.getNumVertices(); i++) {
		pvaluesMountain.push_back((vertMountain[i]).x);
		pvaluesMountain.push_back((vertMountain[i]).y);
		pvaluesMountain.push_back((vertMountain[i]).z);
        tvaluesMountain.push_back((texMountain[i]).s);
		tvaluesMountain.push_back((texMountain[i]).t);
		nvaluesMountain.push_back((normMountain[i]).x);
		nvaluesMountain.push_back((normMountain[i]).y);
		nvaluesMountain.push_back((normMountain[i]).z);
        tavaluesMountain.push_back((tanMountain[i]).x);
        tavaluesMountain.push_back((tanMountain[i]).y);
        tavaluesMountain.push_back((tanMountain[i]).z);
        bivaluesMountain.push_back((bitMountain[i]).x);
        bivaluesMountain.push_back((bitMountain[i]).y);
        bivaluesMountain.push_back((bitMountain[i]).z);
	}
    glBindBuffer(GL_ARRAY_BUFFER, vbo[30]);
	glBufferData(GL_ARRAY_BUFFER, pvaluesMountain.size() * 4, &pvaluesMountain[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[31]);
	glBufferData(GL_ARRAY_BUFFER, tvaluesMountain.size()*4, &tvaluesMountain[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[32]);
	glBufferData(GL_ARRAY_BUFFER, nvaluesMountain.size() * 4, &nvaluesMountain[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[44]);
	glBufferData(GL_ARRAY_BUFFER, tavaluesMountain.size() * 4, &tavaluesMountain[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[43]);
	glBufferData(GL_ARRAY_BUFFER, bivaluesMountain.size() * 4, &bivaluesMountain[0], GL_STATIC_DRAW);


    /* Model : Flat
    *  Vertices
    *  Textures
    *  Normals
    */
    float side= 2000.0f,levelY=0.0f; // lado y pos en el eje y
    float posSide = (float)side/2.0f;
    float flatPositions[18] = // ((6vertex)2 triangles)* 3 floats = 18 floats
	{ -posSide, levelY, posSide, -posSide, levelY, -posSide, posSide, levelY, posSide,
      posSide, levelY, posSide, posSide, levelY, -posSide, -posSide, levelY, -posSide};

    float normalsCoordinates[18] ={
        0.0,1.0,0.0,0.0,1.0,0.0,0.0,1.0,0.0,
        0.0,1.0,0.0,0.0,1.0,0.0,0.0,1.0,0.0};

    float tangentsFlat[18] ={
        1.0,0.0,0.0,1.0,0.0,0.0,1.0,0.0,0.0,
        0.0,0.0,1.0,0.0,0.0,1.0,0.0,0.0,1.0};

	float textureFlatCoordinates[12] = //4vertex
	{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
      1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

    glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(flatPositions), flatPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureFlatCoordinates),textureFlatCoordinates, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normalsCoordinates),normalsCoordinates, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[47]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tangentsFlat),tangentsFlat, GL_STATIC_DRAW);



    //Human
    float cubePositions[108] = //36 vertex, every face using two triangles, 2x6 = 12 triangles x 3 vertex
			{ -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, };

    float normalsCube[108] = {
            0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,
            1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,
            0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,
            -1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,
            0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,
            0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f
    };

    float tangentsCube[108]={
           -1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,
            0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,
            -1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,
            0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,
            0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,
            0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f
    };

    /*float cube2Positions[108] = //36 vertex, every face using two triangles, 2x6 = 12 triangles x 3 vertex
			{ -1.0,-1.0,0.0,1.0,1.0,0.0,1.0,-1.0,0.0,1.0,1.0,0.0,-1.0,-1.0,0.0,-1.0,1.0,0.0,
            1.0,1.0,0.0,1.0,1.0,-1.0,-1.0,1.0,-1.0,-1.0,1.0,0.0,-1.0,1.0,-1.0,1.0,1.0,0.0,
            -1.0,-1.0,0.0,1.0,-1.0,0.0,-1.0,-1.0,-1.0,1.0,-1.0,-1.0,-1.0,-1.0,-1.0,1.0,-1.0,0.0,
            -1.0,-1.0,-1.0,1.0,-1.0,-1.0,-1.0,1.0,-1.0,1.0,1.0,-1.0,1.0,-1.0,-1.0,-1.0,1.0,-1.0,
            -1.0,-1.0,-1.0,-1.0,1.0,-1.0,-1.0,-1.0,0.0,-1.0,1.0,0.0,-1.0,-1.0,0.0,-1.0,1.0,-1.0,
            1.0,-1.0,-1.0,1.0,1.0,-1.0,1.0,-1.0,0.0,1.0,1.0,0.0,1.0,-1.0,0.0,1.0,1.0,-1.0
            };*/

    float cube2Positions[108] = //36 vertex, every face using two triangles, 2x6 = 12 triangles x 3 vertex
			{ -1.0,-1.0,-1.0,-1.0,1.0,-1.0,-1.0,-1.0,0.0,-1.0,1.0,0.0,-1.0,-1.0,0.0,-1.0,1.0,-1.0,
            -1.0,-1.0,0.0,1.0,-1.0,0.0,-1.0,-1.0,-1.0,1.0,-1.0,-1.0,-1.0,-1.0,-1.0,1.0,-1.0,0.0,
            -1.0,-1.0,0.0,1.0,1.0,0.0,1.0,-1.0,0.0,1.0,1.0,0.0,-1.0,-1.0,0.0,-1.0,1.0,0.0,
            1.0,1.0,0.0,1.0,1.0,-1.0,-1.0,1.0,-1.0,-1.0,1.0,0.0,-1.0,1.0,-1.0,1.0,1.0,0.0,
            -1.0,-1.0,-1.0,1.0,-1.0,-1.0,-1.0,1.0,-1.0,1.0,1.0,-1.0,1.0,-1.0,-1.0,-1.0,1.0,-1.0,
            1.0,-1.0,-1.0,1.0,1.0,-1.0,1.0,-1.0,0.0,1.0,1.0,0.0,1.0,-1.0,0.0,1.0,1.0,-1.0
            };

    glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions), cubePositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[13]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube2Positions), cube2Positions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[14]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normalsCube), normalsCube, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[42]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tangentsCube), tangentsCube, GL_STATIC_DRAW);





    /* Model : Sphere(sky)
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

	glBindBuffer(GL_ARRAY_BUFFER, vbo[16]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size()*4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[17]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size()*4, &nvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[50]);
	glBufferData(GL_ARRAY_BUFFER, tangentsvalues.size()*4, &tangentsvalues[0], GL_STATIC_DRAW);



}
