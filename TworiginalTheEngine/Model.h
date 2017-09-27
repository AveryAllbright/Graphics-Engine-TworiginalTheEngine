#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>

#include <vector>
#include <fstream>
#include <sstream>
struct Vertex
{
	glm::vec3 vPosition;
	glm::vec2 vUV;
	glm::vec3 vNormal;
};

struct VertInd
{
	unsigned int locInd;
	unsigned int uvInd;
	unsigned int normInd;
};
class Model
{
private:
	GLuint vertBuf;
	GLuint vertArray;
	unsigned int unVertCount;
	glm::mat4 worldLoc;
	glm::vec3 size;
	std::vector<Vertex> vertBufData;

public:
	char* objFile;

	Model();
	Model(char* objPath);
	bool Buffer();
	void Render();
	virtual ~Model();

	glm::vec3 getSize();
};

