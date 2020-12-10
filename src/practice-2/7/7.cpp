/*
* author : @felipeturing
* exercise : Realizar una animación en el triángulo recto anterior: trasladar el triángulode derecha a izquierda según el espacio de la ventana.
*/

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <string>
#include <fstream>

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

unsigned int numTriangles=1;
/**
unsigned float isn't exists, because :
Unsigned integer types have two important properties that differentiate them from signed integer types: "shifted" range (no negative subrange, but positive subrange twice as wide) and modulo arithmetic. For integer types these properties are important enough to justify the existence of unsigned types.

With floating-types neither of these properties are immediately applicable. With floating-point types the main issue is not in their range (for many purposes it can be thought of as virtually infinite), but rather in precision. And modulo arithmetic is not naturally applicable to non-integer types. For this reason, it didn't make much sense to introduce unsigned floating-point types, i.e. it didn't make much sense to flip-flop the role of just one bit in the floating-point representation.

It should also be noted that the above reasoning should probably be used as rationale behind introducing unsigned integer types (and not introducing unsigned floating-point types) in popular hardware and corresponding hardware-derived standards. What we have in C and C++ was essentially inherited from the hardware capabilities and these standards.

Of course, from the conceptual point if view, it would be quite logical to have unsigned floating-point types in the language, just for the sake of consistency. But, alas they are not there.

-AnT user 187690. stackoverflow
**/

// location of triangle on x axis
float x = 0.0f;
// offset for moving the triangle
float inc = 0.01f;
using namespace std;

void printShaderLog(GLuint shader) {
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*)malloc(len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        cout << "Shader Info Log: " << log << endl;
        free(log);
    }
}

void printProgramLog(int prog) {
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*)malloc(len);
        glGetProgramInfoLog(prog, len, &chWrittn, log);
        cout << "Program Info Log: " << log << endl;
        free(log);
    }
}

bool checkOpenGLError() {
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        cout << "glError: " << glErr << endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

string readShaderSource(const char *filePath) {
    string content = "";
    ifstream fileStream(filePath, ios::in);
    string line = "";
    while (getline(fileStream, line)){
//        getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}

GLuint createShaderProgram() {
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;

    string vertShaderStr = readShaderSource("vs.glsl");
    string fragShaderStr = readShaderSource("fs.glsl");

    const char* vertShaderSrc = vertShaderStr.c_str();
    const char* fragShaderSrc = fragShaderStr.c_str();

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vertShaderSrc, NULL);
    glShaderSource(fShader, 1, &fragShaderSrc, NULL);

    glCompileShader(vShader);
    checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if (vertCompiled != 1) {
        cout << "vertex compilation failed" << endl;
        printShaderLog(vShader);
    }

    glCompileShader(fShader);
    checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != 1) {
        cout << "fragment compilation failed" << endl;
        printShaderLog(fShader);
    }

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);

    glLinkProgram(vfProgram);
    checkOpenGLError();
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if (linked != 1) {
        cout << "linking failed" << endl;
        printProgramLog(vfProgram);
    }
    return vfProgram;
}


void init (GLFWwindow* window) {
    renderingProgram = createShaderProgram();
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime) {
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);

    x = x + inc; //x += inc;

    if(x > 1.0f) inc = -0.01f;
    if(x < -1.0f) inc = 0.01f;

    // Crea una variable uniforme para que cualquier proceso del pipeline(vertexshader, tesellation, geometryshader, fragmentshader, etc) pueda manipularla.
    GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset");

    // Dale un x(float) a la variable offsetLoc
    glProgramUniform1f(renderingProgram, offsetLoc, x);

    glDrawArrays(GL_TRIANGLES, 0, numTriangles*3);
}

int main(void) {
    if (!glfwInit()) {exit(EXIT_FAILURE);}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(600, 600, "1.1", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {exit(EXIT_FAILURE);}
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