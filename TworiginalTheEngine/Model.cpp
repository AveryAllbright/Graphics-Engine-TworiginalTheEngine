#include "Model.h"
const GLuint POSITION_LOCATION = 0;
const GLuint UV_LOCATION = 1;
const GLuint NORMAL_LOCATION = 2;

Model::Model()
{
	//I exist because the compiler needs me to
}

Model::Model(char * objPath)
{
	vertBuf = 0;
	vertArray = 0;
	objFile = objPath;
}

bool Model::Buffer()
{
	std::vector<glm::vec3> locs;	//Position (model-space) of each point on the model
	std::vector<glm::vec2> uvs;		//Position (model-space) of each UV per vertex
	std::vector<glm::vec3> normals; //Position (model-space) of each Normal per vertex
	std::vector<VertInd> index;

	std::ifstream inFile;
	inFile.open(objFile);

	//Read in the Obj File
#pragma region LoadFile
	std::string line;
	if (!inFile.is_open()) { return false; }

	float minX, minY, minZ;			//Minimum value for the model (the lowest point)
	float maxX, maxY, maxZ;			//Maximum value for the model (the highest point in the model)
	minX = minY = minZ = INT32_MAX;	//set to Max; every point is lower
	maxX = maxY = maxZ = -INT32_MAX;//set to Min; every point is higher
	float width, height, depth;		//overall size of the model (not the scale) this is used to determine the collider size for the model

	while (std::getline(inFile, line))
	{
		std::stringstream file(line);
		std::string label;
		file >> label;

		if (label == "v") //vertex
		{
			float x, y, z;
			file >> x >> y >> z;
			locs.push_back(glm::vec3(x, y, z));
			if (x < minX) { minX = x; }
			if (y < minY) { minY = y; }
			if (z < minZ) { minZ = z; }
			if (x > maxX) { maxX = x; }
			if (y > maxY) { maxY = y; }
			if (z > maxZ) { maxZ = z; }
		}
		else if (label == "vt") //UV
		{
			float x, y;
			file >> x >> y;
			uvs.push_back(glm::vec2(x, y));
		}

		else if (label == "vn") //normal
		{
			float x, y, z;
			file >> x >> y >> z;
			normals.push_back(glm::vec3(x, y, z));
		}
		else if (label == "f") //Fragment (a set of verts, a UV, and a normal that makes up each triangle)
		{
			for (int i = 0; i < 3; i++)
			{
				unsigned int x, y, z;
				char slash;
				file >> x >> slash >> y >> slash >> z;
				x--;
				y--;
				z--;
				VertInd vertInd;
				vertInd.locInd = x;
				vertInd.uvInd = y;
				vertInd.normInd = z;
				index.push_back(vertInd);
			}
		}
	}

	//The overall size of the model is its highest point minus its lowest point
	width = maxX - minX;
	height = maxY - minY;
	depth = maxZ - minZ;

	glm::vec3 vCenter = glm::vec3();

	//Since a proper model is centered at the origin, it should be half below and half above
	//vCenter should return 0, 0, 0; Otherwise...
	vCenter.x = (maxX + minX) / 2;
	vCenter.y = (maxY + minY) / 2;
	vCenter.z = (maxZ + minZ) / 2;

	//... by subtracting the value it actually is, we shift the model to be centered at the origin
	for (int i = 0; i < locs.size(); i++)
	{
		locs[i] -= vCenter;
	}

	size = glm::vec3(width, height, depth);

	inFile.close();
#pragma endregion

	//Once a file is read, the data must be passed to the shader, and be remade into a real model
#pragma region Buffer Control
	unVertCount = index.size();
	vertBufData.resize(unVertCount);

	//for every vertex, create a vertex containing the location (position), uv, and normal for that vertex
	for (unsigned int i = 0; i < unVertCount; i++)
	{
		vertBufData[i] =
		{
			locs[index[i].locInd],
			uvs[index[i].uvInd],
			normals[index[i].normInd]
		};
	}

	glGenVertexArrays(1, &vertArray);		//generate the active array of vertices (aka the Object)
	glGenBuffers(1, &vertBuf);

	glBindVertexArray(vertArray);			//bind the array and buffer as active, so the GPU knows to draw these triangles
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);

	glBufferData(GL_ARRAY_BUFFER,			//Where to copy to
		sizeof(Vertex) * unVertCount,		//How many bytes? : one vertex worth * how many vertices in the model
		&vertBufData[0],					//Where to start from? : the first vertex in the model
		GL_STATIC_DRAW);					// Models for this engine have no animations, deformations, etc, and therefore are static, only having to be buffered once

	//Enable these locations within the shader
	glEnableVertexAttribArray(POSITION_LOCATION);
	glEnableVertexAttribArray(UV_LOCATION);
	glEnableVertexAttribArray(NORMAL_LOCATION);

	glVertexAttribPointer(		//Position
		POSITION_LOCATION,		//Attribute Index for Position
		3,						//How many components will be read in? : x y z
		GL_FLOAT,				//What type of data? : Floats
		GL_FALSE,				// Data Normalization?
		sizeof(Vertex),			//Stride : bytes of data
		0);						//Offset to this byte

	glVertexAttribPointer(		//UV
		UV_LOCATION,
		2,						//How many components will be read in? : x y
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)));	//Offset the Stride from the start of a vertex by three floats (pos)

	glVertexAttribPointer(		//Normal
		NORMAL_LOCATION,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));	//Offset the Stride from the start of a vertex by 5 floats (pos) + (uv)

	glBindVertexArray(0);	//Done working with the model until we render, so reset the active array to null (0)
#pragma endregion

	return true;
}

void Model::Render()
{
	glBindVertexArray(vertArray);
	glDrawArrays(GL_TRIANGLES, 0, unVertCount);
}

Model::~Model()
{
	//Nothing to do here
}

glm::vec3 Model::getSize()
{
	return size;
}
