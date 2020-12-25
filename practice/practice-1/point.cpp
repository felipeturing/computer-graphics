/**
author      : @felipeturing
description : Display a point on a window
**/

/**
glew      : Es una extesion-loader-library, manejador de paquetes de opengl.
glad      : Es una extesion-loader-library, al igual que GLEW.
glad/gl.h : Cargar todo el entorno necesario para <<gl>> (opengl)
que sean compatibles con la plataforma que estamos usando.
**/

#include <GL/glew.h>

/**
GLFW         : Es una biblioteca que permita la creación y manejo de ventanas.
GLFW/glfw3.h : Proporciona todas las constantes, tipos de datos y funciones de la GLFW API.
Por defecto incluye todas las librerías de OpenGL,
sin embargo en algunas plataformas solo soporta versiones antiguas,
por ejemplo en Windows, donde usualmente solo soporta OpenGL 1.2,
para no tener problemas con eso en su lugar se usa una extesion-loader-library, por ejemplo glad, como estamos usando nosotros.
GLFW puede detectar tal include y no volver a incluirlas.

Para asegurarnos que no hay conflictos en los includes, es decir con las cabeceras podemos definir GLFW_INCLUDE_NONE antes del encabezado GLFW para explicitamente deshabilitar la inclusión de los encabezados de OpenGL.
**/
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

/**
stdlib.h, stdio.h y iostream son parte del encabezado estandar para el manejo de
entradas y salidas y muchas otras funcionalidades.
**/
#include<stdio.h>
#include<stdlib.h>
#include<iostream>

using namespace std;

#define numVAOs 1

// GLuint : unsigned int
GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint createShaderProgram() {


    const char* vshaderSource =
    "#version 410 \n"
    "void main(void) \n"

    "{gl_Position = vec4(0.0, 0.0, 0.0, 1.0);}";

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


void init (GLFWwindow* window) {
    renderingProgram = createShaderProgram();
o
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime) {

    glUseProgram(renderingProgram);

    glPointSize(30.0f);

    glDrawArrays(GL_POINTS, 0, 1);
}

int main(void) {
    if (!glfwInit()) {exit(EXIT_FAILURE);}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow *window = glfwCreateWindow(800, 500, "A Point is here!, Hi mom.", NULL,NULL);
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
