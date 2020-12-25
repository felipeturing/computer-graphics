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

using namespace std;

#define numVAOs 1
#define numVBOs 2

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
float zrot = 0.0f;
float inc = 0.01f;
glm::mat4 pMat, vMat, mMat, mvMat;
glm::vec4 posArm, posArm2;
stack<glm::mat4> mvStack;

void setupVertices(void) {
    // 12 triangles * 3 vertices * 3 values (x, y, z)
    float cubePositions[108] = {
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
    };

    // we need at least 1 VAO and 2 VBOs
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions), cubePositions, GL_STATIC_DRAW);

}

void init (GLFWwindow* window) {
    renderingProgram = createShaderProgram("vs.glsl", "fs.glsl");

    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians == 60 degrees

    // position the camera further down the positive Z axis (to see all of the cubes)
    cameraX = 0.0f;
    cameraY = 10.0f;
    cameraZ = 32.0f;
    setupVertices();
}

// repeatedly
void display(GLFWwindow* window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);

    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    // push view matrix onto the stack
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    mvStack.push(vMat);
        // brazo 1
        mvStack.push(mvStack.top());
            mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime/8.0f, glm::vec3(0.0, 0.0, 1.0));
            mvStack.push(mvStack.top());
                mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(6.0f, 1.0f, 1.0f));  // arm scale
                mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));  // arm

                glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
                //
                glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(0);
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LEQUAL);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            mvStack.pop();

            // brazo 2
            posArm = glm::vec4(12, 0.5, -0.5, 1.0);
            mvStack.push(mvStack.top());
                mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(posArm));
                mvStack.push(mvStack.top());

                    zrot += inc;

                    if (zrot > M_PI/4.0)
                        inc = -0.01;
                    else if (zrot < M_PI/-4.0)
                        inc = 0.01;

                    mvStack.top() *= glm::rotate(glm::mat4(1.0f), zrot, glm::vec3(0.0, 0.0, 1.0));
                    mvStack.push(mvStack.top());
                        mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(4.0f, 1.0f, 1.0f));
                        mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));

                        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
                        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
                        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
                        glEnableVertexAttribArray(0);

                        glDrawArrays(GL_TRIANGLES, 0, 36);
                    mvStack.pop();

                mvStack.pop();
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
    if (!glfwInit()) {exit(EXIT_FAILURE);}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // I don't know what this does
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // and neither this
    GLFWwindow* window = glfwCreateWindow(600, 600, "brazo robot", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {exit(EXIT_FAILURE);}
    glfwSwapInterval(1);

    glfwSetWindowSizeCallback(window, window_size_callback);

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
