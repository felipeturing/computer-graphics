/**
author      : @felipeturing
description : Before we try writing shaders, let’s write a simple C++/OpenGL application
that instantiates a GLFWwindow and sets its background color, display a light blue window with size 800*500px.
book        : Computer Graphics Programming in OpenGL with C++ - V.Scoot Gordon and John Clevenger.
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

/*
    GLFWwindow : objeto que representa la ventana
*/
void init(GLFWwindow *window) {
}

void display(GLFWwindow *window, double currentTime) {
    /**
    glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha):
    parameters  : Rojo, verde, azul, alpha son valores usados cuando el buffer de color esta limpio. Los valores iniciales son 0;
    description : Especifica los parametros usados por glClear(.) para limpiar el buffer de color.
    valores espeficados por glClearColor(.) estan dentro del intervalo [0,1]
    **/
	glClearColor(0.0, 1.0, 1.0, 1.0);

    /**
    GL_COLOR_BUFFER_BIT : Referencia al color del buffer.
    glClear(GL_COLOR_BUFFER_BIT)
    description: Limpia el color buffer y asigna el color especificado above.
    **/
	glClear(GL_COLOR_BUFFER_BIT);
}

int main(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

    /**
    Versión 2 de OpenGL
    **/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    /**
    GLFWwindow* glfwCreateWindow(int width, int height, const char * title, GLFWmonitor * monitor, GLFWwindow * share)
    description : Esta función crea una ventana y su contexto asociado de OpenGL. La mayoría de las opciones que controlan como las
    ventanas y sus contextos deberían crearse estan en window hints.
    **/
	GLFWwindow *window = glfwCreateWindow(800, 500, "GLFW is here!, Hi mom.", NULL,NULL);

    /**
    glfwMakeContextCurrent( GLFWwindow * 	window)
    Esta función hace que el contexto OpenGL o OpenGL ES de la ventana especificada esté actualmente en el hijo de llamada.
    Un contexto solo debe estar actualizado en un solo hilo a la vez y cada hilo puede tener solo un único contexto actual a la vez.
    Al mover un contexto entre hilos, debe convertirlo en no actual en el hilo antiguo antes de hacerlo actual en el nuevo
    **/
	glfwMakeContextCurrent(window);

    /**
    **/
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

    /**
    glfwSwapInterval(int intervalodetiempo)
    Esta función establece el intervalo de intercambio para el contexto actual de OpenGL o OpenGL ES, es decir, el número de
    actualizaciones de pantalla que deben esperar desde el momento en que se llamó a glfwSwapBuffers antes de intercambiar los
    búferes y regresar. Esto a veces se denomina sincronización vertical , sincronización de retroceso vertical o simplemente
    vsync .
    **/
	glfwSwapInterval(1);

    /**
    Iniciar la ventana
    **/
	init(window);


    /**
    Refrescar la pantalla
    **/
	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwDestroyWindow(window);
	glfwTerminate();
    exit(EXIT_SUCCESS);
}
