#include <cmath>
#include <vector>
#include <glm/glm.hpp>

class Cylinder
{
private:
	int numVertices;
	int numIndices;
	std::vector<int> indices;
	std::vector<glm::vec3> vertices;
	//std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	//std::vector<glm::vec3> tangents;
	void init(int);
	float toRadians(float degrees);

public:
	Cylinder();
	Cylinder(int prec);
	int getNumVertices();
	int getNumIndices();
	std::vector<int> getIndices();
	std::vector<glm::vec3> getVertices();
	//std::vector<glm::vec2> getTexCoords();
	std::vector<glm::vec3> getNormals();
	//std::vector<glm::vec3> getTangents();
};