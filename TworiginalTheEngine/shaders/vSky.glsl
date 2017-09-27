#version 430

layout (location = 0) in vec3 vertPos;
layout (location = 4) uniform mat4 camMat;

out vec3 direc;

void main()
{
	gl_Position = camMat * vec4(vertPos, 1);
	direc = vertPos;
}