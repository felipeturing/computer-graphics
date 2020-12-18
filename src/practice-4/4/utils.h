
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <opencv2/opencv.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

void loadTexture(const char *texImagePath, GLuint& imageTexture1)
{
    cv::Mat image = cv::imread(texImagePath);

    if( image.empty() ){
      std::cout << "image empty" << std::endl;
    } else {
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glGenTextures(1, &imageTexture1);
      glBindTexture(GL_TEXTURE_2D, imageTexture1);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (CV_VERSION_MAJOR >= 4) cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    else printf("Su versión de OpenGL < 4, por favor cambie cv::COLOR_BGR2RGB por CV_BGR2RGB en utils.h:42");

      glTexImage2D(GL_TEXTURE_2D,         // Type of texture
                     	0,                   // Pyramid level (for mip-mapping) - 0 is the top level
			            GL_RGB,              // Internal colour format to convert to
                     	image.cols,          // Image width  i.e. 640 for Kinect in standard mode
                     	image.rows,          // Image height i.e. 480 for Kinect in standard mode
                     	0,                   // Border width in pixels (can either be 1 or 0)
			            GL_RGB,              // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
			            GL_UNSIGNED_BYTE,    // Image data type
			            image.ptr());        // The actual image data itself
	}
}

string readShaderFile(const char *filePath) {
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
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

void printShaderLog(GLuint shader) {
	int len = 0;
	int chWrittn = 0;
	char *log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char *)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
}
void printProgramLog(int prog) {
	int len = 0;
	int chWrittn = 0;
	char *log;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char *)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		cout << "Program Info Log: " << log << endl;
		free(log);
	}
}
string readShaderSource(const char *filePath) {
    string content = "";
    ifstream fileStream(filePath, ios::in);
    //~ cout << "Error: " << strerror(errno) << endl;  // No such file or directory
    //~ cout << fileStream.is_open() << endl;  // 0
    string line = "";
    while (getline(fileStream, line))
        content.append(line + "\n");

    fileStream.close();
    return content;
}
GLuint createShaderProgram(const char *vshaderfile, const char *fshaderfile) {
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;

    string vertShaderStr = readShaderSource(vshaderfile);
    string fragShaderStr = readShaderSource(fshaderfile);

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
