#version 430

layout (location = 0) in vec3 position;
layout (location = 1) uniform mat4 cameraMat;
out float blue;
void main()

{
	gl_Position = cameraMat * vec4(position,1);
	blue = (gl_Position.x + 1) / 2;
}