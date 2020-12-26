#include <cmath>
#include <vector>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Cono.h"
using namespace std;

Cono::Cono() {
	init(48);
}

Cono::Cono(int prec) {
	init(prec);
}

float Cono::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Cono::init(int prec) {
	numVertices = (prec + 1) * (prec + 1);
	numIndices = prec * prec * 6;
	for (int i = 0; i < numVertices; i++) { vertices.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { normals.push_back(glm::vec3()); }
	for (int i = 0; i < numIndices; i++) { indices.push_back(0); }

	// calculate the first triangle to rotate
	    glm::vec3 p1(0.0f,0.0f,0.0f);
        glm::vec3 p2(1.5f,0.0f,0.0f);
	    glm::vec3 p3(0.0f,5.0f,0.0f);

	    vertices[0] = glm::vec3(p1);
        vertices[1] = glm::vec3(p2);
        vertices[2] = glm::vec3(p3);

    // rotate the first triangle about Y to get the other triangles
	for (int triangle = 1; triangle < prec + 1; triangle++) {
		for (int i = 0; i < 3; i++) {
			float amt = (float)toRadians((float)triangle * 360.0f / (prec));

			glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
			vertices[triangle*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(vertices[i], 1.0f));
		}
	}

    //triangles indices
	for (int triangle = 0; triangle < prec; triangle++) {
		for (int i = 0; i < 3; i++) {
			indices[((triangle*prec + i) * 2) * 3 + 0] = triangle*(prec + 1) + i;
			indices[((triangle*prec + i) * 2) * 3 + 1] = (triangle + 1)*(prec + 1) + i;
			indices[((triangle*prec + i) * 2) * 3 + 2] = triangle*(prec + 1) + i + 1;
			indices[((triangle*prec + i) * 2 + 1) * 3 + 0] = triangle*(prec + 1) + i + 1;
			indices[((triangle*prec + i) * 2 + 1) * 3 + 1] = (triangle + 1)*(prec + 1) + i;
			indices[((triangle*prec + i) * 2 + 1) * 3 + 2] = (triangle + 1)*(prec + 1) + i + 1;
		}
	}
}

int Cono::getNumVertices() { return numVertices; }
int Cono::getNumIndices() { return numIndices; }
std::vector<int> Cono::getIndices() { return indices; }
std::vector<glm::vec3> Cono::getVertices() { return vertices; }
//std::vector<glm::vec2> Sphere::getTexCoords() { return texCoords; }
std::vector<glm::vec3> Cono::getNormals() { return normals; }
