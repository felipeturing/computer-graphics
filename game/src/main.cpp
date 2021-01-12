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
#include "Utils.h"
#include "Sphere.h"
#include "ImportedModel.h"

using namespace std;

//VAOs and VBOs
#define numVAOs 1
#define numVBOs 23

// global variables
float cameraX, cameraY, cameraZ, RockLocX, RockLocY, RockLocZ, HouseLocX, HouseLocY, HouseLocZ, FlatLocX, FlatLocY, FlatLocZ, SkyLocX, SkyLocY, SkyLocZ, aspect, mountainLocX,  mountainLocY, mountainLocZ, humanPosZ, humanPosY, humanPosX, closeto, angleCamera, angleCameraInc, step, incRotLeg1, incRotLeg2, rotLeg1, rotLeg2, rotLeg3, incRotLeg3, rotArm, incRotArm, rotSky, incRotSky, rotBodyHuman, incRotBodyHuman, incJumping;

GLuint renderingProgram, vao[numVAOs], vbo[numVBOs], mvLoc, projLoc, obj, rockTexture, houseTexture, flatTexture,houseNavTexture, mountainTexture, skyTexture;

int width, height, keyboard, actionKeyboard;

bool walkingBool,jumpingBool;

glm::mat4 pMat, vMat, mMat, mvMat;
glm::vec4 posLeftLeg, posRightLeg, posTrunk, posHead, posLeftArm, posRightArm;

stack<glm::mat4> mvStack;

// model and geometry
ImportedModel Rock("../models/Rock_big_single_b_LOD0.obj");
ImportedModel House("../models/house.obj");
ImportedModel Mountain("../models/mountain/Mountain.obj");
Sphere mySphere = Sphere(48);


// prototypes
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



void init(GLFWwindow* window) {
    renderingProgram = createShaderProgram("shaders/vs.glsl", "shaders/fs.glsl");
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
    rotSky = 0.0f; incRotSky = (float)M_PI/5000.0f;incJumping = 0.5f;

    jumpingBool = false;walkingBool=false;

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.3472f, aspect, 0.1f, 1000000.0f);
    pMat = glm::rotate(pMat, angleCamera ,glm::vec3(1.0,0.0,0.0));

    setupVertices();

    loadTexture("../textures/Rock_big_single_b_diffuse_desert.jpg", rockTexture);
    loadTexture("../textures/house/house_diffuse.jpg", houseTexture);
    loadTexture("../textures/Rock_big_single_b_sandstone_flat.jpg", flatTexture);
    loadTexture("../textures/mountain/Color.png", mountainTexture);
    loadTexture("../textures/sky/sky4.jpg", skyTexture);
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    obj = glGetUniformLocation(renderingProgram, "obj");


    vMat = glm::lookAt(glm::vec3(cameraX, cameraY, cameraZ)+glm::vec3(humanPosX, humanPosY,humanPosZ),
						glm::vec3(humanPosX, humanPosY,humanPosZ),
						glm::vec3(0.0f, 10.0f, 0.0f));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    sceneViewNavigation();
    //sceneMountain();
    sceneDessert();
    //humanBasicAnimation();
    humanAdvancedAnimation();
}

//definitions
void sceneDessert(void){
    //rocks
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(RockLocX, RockLocY, RockLocZ));
	mvMat = vMat * mMat;
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
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
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(HouseLocX, HouseLocY+30.0, HouseLocZ));
    mMat = glm::rotate(mMat, 2.5f, glm::vec3(0.0,1.0,0.0));
    mMat = glm::scale(mMat, glm::vec3(2.0,2.0,2.0));
	mvMat = vMat * mMat;
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
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

    //Sky
    rotSky += incRotSky;
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(SkyLocX, SkyLocY, SkyLocZ));
    mMat = glm::rotate(mMat, rotSky, glm::vec3(0.0,1.0,0.0));
    mMat = glm::scale(mMat, glm::vec3(10000.0,10000.0,10000.0));
    mvMat = vMat * mMat;
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[15]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[16]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,skyTexture);
    glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
}

void sceneMountain(void){
    //mountain
    /*mMat = glm::translate(glm::mat4(1.0f), glm::vec3(mountainLocX, mountainLocY, mountainLocZ));
    mMat = glm::scale(mMat,glm::vec3(100.0,100.0,100.0));
	mvMat = vMat * mMat;
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
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


void humanAdvancedAnimation(void){

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
    glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
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
        /*if (rotArm > M_PI/8.0){ // Rotación de brazos de -M_PI/8 hasta M_PI/8
            incRotArm = -0.05/2.0;
        }
        else if (rotArm < -M_PI/8.0){
            incRotArm = 0.05/2.0;
        }*/

        //if(walkingBool == 1){rotArm += incRotArm;}
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
    /*if (rotLeg1 > M_PI/8.0){
        incRotLeg1 = -0.05/2.0;
    }
    else if (rotLeg1 < -M_PI/8.0){
        incRotLeg1 = 0.05/2.0;
    }*/
    //if(walkingBool == 1){rotLeg1 += incRotLeg1;}
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

                /*if(rotLeg2 > M_PI/4.0f){
                    incRotLeg2 = 0.0;
                }
                if(rotLeg1 < -M_PI/8.0){
                    incRotLeg2 = -2.0*(0.05/2.0);
                }
                if(rotLeg2 < 0.0){
                    incRotLeg2 = 2.0*(0.05/2.0);
                }*/
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

                /*if(rotLeg3 > M_PI/4.0f){
                    incRotLeg3 = -2.0*(0.05/2.0);
                }
                if(rotLeg1 > M_PI/8.0){
                    incRotLeg3 = 2.0*(0.05/2.0);
                }
                if(rotLeg1 < -M_PI/8.0){
                    incRotLeg3 = 0.0;
                }*/

                //if(walkingBool == 1){rotLeg3 += incRotLeg3;}
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


void humanBasicAnimation(void){
    //Human
    //head
    /*if(humanPosZ > 500.0){
        step = -0.5f;
    }else if(humanPosZ < -500.0){
        step = 0.5f;
    }
    //step = 0.0; //sin traslado
    humanPosZ += step;
    humanPosX = 0.0;
    humanPosY = 25.0f;

    //la posición del humano es la de su cabeza
    glUniform1i(obj,43);
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
    glUniform1i(obj,42);
    posTrunk = glm::vec4(0.0,-7.0,0.0,1.0); // posicion del tronco respecto de la cabeza
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(posTrunk));
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(5.0,5.0,2.0));
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[13]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    mvStack.pop();

    // arms
    glUniform1i(obj,43);
    if (rotArm > M_PI/4.0){
        incRotArm = -0.05;
    }
    else if (rotArm < -M_PI/4.0){
        incRotArm = 0.05;
    }
    rotArm += incRotArm;

    //left arm
    posLeftArm = glm::vec4(6.0,5.2,0.0,1.0);
    mvStack.push(mvStack.top());
    mvStack.top()  *= glm::translate(glm::mat4(1.0f), glm::vec3(posLeftArm));
    // Start : Manipulation the own referene system of left arm
    mvStack.top()  *= glm::rotate(glm::mat4(1.0f), (float)M_PI/64.0f, glm::vec3(0.0,0.0,1.0));
    mvStack.top()  *= glm::rotate(glm::mat4(1.0f), rotArm, glm::vec3(1.0,0.0,0.0));
    mvStack.top()  *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-5.0,0.0));
    mvStack.top()  *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0,4.0,2.0));
    // End
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[13]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    mvStack.pop();

    //right arm
    posRightArm = glm::vec4(-6.0,5.2,-0.25,1.0);
    mvStack.push(mvStack.top());
    mvStack.top()  *= glm::translate(glm::mat4(1.0f), glm::vec3(posRightArm));
    // Start : Manipulation the own referene system of left arm
    mvStack.top()  *= glm::rotate(glm::mat4(1.0f), -(float)M_PI/64.0f, glm::vec3(0.0,0.0,1.0));
    mvStack.top()  *= glm::rotate(glm::mat4(1.0f), -rotArm, glm::vec3(1.0,0.0,0.0));
    mvStack.top()  *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-5.0,0.0));
    mvStack.top()  *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0,4.0,1.5));
    // End
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[13]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    mvStack.pop();

    //left leg
    if (rotLeg > M_PI/8.0){
            incRotLeg = -0.05/2.0;
    }
    else if (rotLeg < -M_PI/8.0){
            incRotLeg = 0.05/2.0;
    }
    rotLeg += incRotLeg;
    posLeftLeg = glm::vec4(2.0,-4.0,0.0,1.0);
    mvStack.push(mvStack.top());
    mvStack.top()  *= glm::translate(glm::mat4(1.0f), glm::vec3(posLeftLeg));
    // Start : Manipulation the own referene system of left leg
    mvStack.top()  *= glm::rotate(glm::mat4(1.0f), -rotLeg, glm::vec3(1.0,0.0,0.0));
    mvStack.top()  *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-5.0,0.0));
    mvStack.top()  *= glm::scale(glm::mat4(1.0f), glm::vec3(1.2,4.0,2.0));
    // End
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[13]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    mvStack.pop();

    //right leg
    posRightLeg = glm::vec4(-2.0,-4.0,0.0,1.0);
    mvStack.push(mvStack.top());
    mvStack.top()  *= glm::translate(glm::mat4(1.0f), glm::vec3(posRightLeg));
    // Start : Manipulation the own referene system of right leg
    mvStack.top()  *= glm::rotate(glm::mat4(1.0f), rotLeg, glm::vec3(1.0,0.0,0.0));
    mvStack.top()  *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0,-5.0,0.0));
    mvStack.top()  *= glm::scale(glm::mat4(1.0f), glm::vec3(1.2,4.0,2.0));
    // End
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[13]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mvStack.pop();
    mvStack.top();
    mvStack.top();
    mvStack.top();*/
}


int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow *window;
	window = glfwCreateWindow(1200, 700, "TAU v0.01", NULL, NULL);

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
    /*if (keyboard == 87 && (actionKeyboard==GLFW_PRESS || actionKeyboard == GLFW_REPEAT) ){ //W
        cameraZ -= closeto;
    }else if (keyboard == 83 && (actionKeyboard==GLFW_PRESS || actionKeyboard == GLFW_REPEAT)){ //S
        cameraZ += closeto;
    }else*/
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

    float cube2Positions[108] = //36 vertex, every face using two triangles, 2x6 = 12 triangles x 3 vertex
			{ -1.0,-1.0,0.0,1.0,1.0,0.0,1.0,-1.0,0.0,1.0,1.0,0.0,-1.0,-1.0,0.0,-1.0,1.0,0.0,
            1.0,1.0,0.0,1.0,1.0,-1.0,-1.0,1.0,-1.0,-1.0,1.0,0.0,-1.0,1.0,-1.0,1.0,1.0,0.0,
            -1.0,-1.0,0.0,1.0,-1.0,0.0,-1.0,-1.0,-1.0,1.0,-1.0,-1.0,-1.0,-1.0,-1.0,1.0,-1.0,0.0,
            -1.0,-1.0,-1.0,1.0,-1.0,-1.0,-1.0,1.0,-1.0,1.0,1.0,-1.0,1.0,-1.0,-1.0,-1.0,1.0,-1.0,
            -1.0,-1.0,-1.0,-1.0,1.0,-1.0,-1.0,-1.0,0.0,-1.0,1.0,0.0,-1.0,-1.0,0.0,-1.0,1.0,-1.0,
            1.0,-1.0,-1.0,1.0,1.0,-1.0,1.0,-1.0,0.0,1.0,1.0,0.0,1.0,-1.0,0.0,1.0,1.0,-1.0
            };

    glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions), cubePositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[13]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube2Positions), cube2Positions, GL_STATIC_DRAW);

    //SPHERE
    std::vector<int> ind = mySphere.getIndices();
	std::vector<glm::vec3> vert = mySphere.getVertices();
	std::vector<glm::vec2> tex = mySphere.getTexCoords();
	std::vector<glm::vec3> norm = mySphere.getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

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
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo[15]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size()*4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[16]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size()*4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[17]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size()*4, &nvalues[0], GL_STATIC_DRAW);
}
