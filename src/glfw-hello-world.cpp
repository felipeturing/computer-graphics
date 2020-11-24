/**
glad      : Es una extesion-loader-library. 
glad/gl.h : Cargar todo el entorno necesario para <<gl>> (opengl) 
que sean compatibles con la plataforma que estamos usando. 
**/

#include <glad/gl.h> 

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
sdtlib.h y stdio.h
**/

