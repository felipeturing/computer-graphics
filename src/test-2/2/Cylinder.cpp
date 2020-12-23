#include <cmath>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include "Cylinder.h"
using namespace std;

Cylinder::Cylinder() {
	init(48);
}

Cylinder::Cylinder(int prec) {
	init(prec);
}

float Cylinder::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Cylinder::init(int prec) {
	numVertices = (prec + 1) * (prec + 1);
	numIndices = prec * prec * 6;
	for (int i = 0; i < numVertices; i++) { vertices.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { normals.push_back(glm::vec3()); }
	for (int i = 0; i < numIndices; i++) { indices.push_back(0); }

	// calculate triangle vertices
	for (int i = 0; i <= prec; i++) {
		for (int j = 0; j <= prec; j++) {
            // altura
			float y = (float)cos(toRadians(180.0f - i * 180.0f / prec));
            //circunferencia
			float x = -(float)cos(toRadians(j*360.0f / (float)prec));
			float z = (float)sin(toRadians(j*360.0f / (float)(prec)));

			vertices[i*(prec + 1) + j] = glm::vec3(x, y, z);
			normals[i*(prec + 1) + j] = glm::vec3((float)x/sqrt(x*x + y*y),(float) y/sqrt(x*x + y*y), 0.0);
		}
	}
	// calculate triangle indices
	for (int i = 0; i<prec; i++) {
		for (int j = 0; j<prec; j++) {
			indices[6 * (i*prec + j) + 0] = i*(prec + 1) + j;
			indices[6 * (i*prec + j) + 1] = i*(prec + 1) + j + 1;
			indices[6 * (i*prec + j) + 2] = (i + 1)*(prec + 1) + j;
			indices[6 * (i*prec + j) + 3] = i*(prec + 1) + j + 1;
			indices[6 * (i*prec + j) + 4] = (i + 1)*(prec + 1) + j + 1;
			indices[6 * (i*prec + j) + 5] = (i + 1)*(prec + 1) + j;
		}
	}
}

int Cylinder::getNumVertices() { return numVertices; }

int Cylinder::getNumIndices() { return numIndices; }

std::vector<int> Cylinder::getIndices() { return indices; }
std::vector<glm::vec3> Cylinder::getVertices() { return vertices; }
//std::vector<glm::vec2> Sphere::getTexCoords() { return texCoords; }
std::vector<glm::vec3> Cylinder::getNormals() { return normals; }
