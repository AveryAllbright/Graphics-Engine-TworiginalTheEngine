#version 430

layout (location = 3) uniform float red;
layout (location = 4) uniform float green;
layout (location = 5) uniform float alpha;
in float blue;

void main()

{
	gl_FragColor = vec4(red, green, blue, alpha);
}