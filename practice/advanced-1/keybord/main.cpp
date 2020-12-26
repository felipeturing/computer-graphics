#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
using namespace std;
#define numVAOs 1

int keyPressed=0;
GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint createShaderProgram() {

    const char* vshaderSource =
    "#version 410 \n"
    "void main(void) \n"

    "{gl_Position = vec4(0.0, (gl_VertexID+1.0)/5.0, 0.0, 1.0);}";

    const char* fshaderSource =
    "#version 410 \n"
    "out vec4 color; \n"
    "void main(void) \n"
    "{color = vec4(0.0, 0.0, 1.0, 1.0);}";

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);


    glShaderSource(vShader, 1, &vshaderSource, NULL);
    glShaderSource(fShader, 1, &fshaderSource, NULL);


    glCompileShader(vShader);
    glCompileShader(fShader);


    GLuint vfProgram = glCreateProgram();

    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);

    glLinkProgram(vfProgram);

    return vfProgram;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

void init (GLFWwindow* window) {
    renderingProgram = createShaderProgram();
    glGenVertexArrays(numVAOs, vao);

    glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);
    glPointSize(30.0f);
    if(keyPressed == 65){ //A
        glDrawArrays(GL_POINTS, 0, 5);
    }else if (keyPressed == 87){ //W
        glDrawArrays(GL_POINTS, 0, 2);
    }else if (keyPressed == 83){ //S
        glDrawArrays(GL_POINTS, 0, 3);
    }else if(keyPressed == 68 ){ //D
        glDrawArrays(GL_POINTS, 0, 4);
    }else{
        glDrawArrays(GL_POINTS, 0, 1);
    }
}

int main(void) {
    if (!glfwInit()) {exit(EXIT_FAILURE);}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window;

	window = glfwCreateWindow(800, 500, "Keyboard", NULL,NULL);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {exit(EXIT_FAILURE);}                // without the line 80 or 81, this makes an error
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

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
    //std::cout << key <<std::endl;
    if(key ==GLFW_KEY_SPACE){
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
    }
    //keyPressed = key;
}
