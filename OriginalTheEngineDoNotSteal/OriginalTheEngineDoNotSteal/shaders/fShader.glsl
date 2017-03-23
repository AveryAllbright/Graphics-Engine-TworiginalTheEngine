#version 430

layout (location = 3) uniform vec4 colour;

void main()

{
	gl_FragColor = colour;
}