#version 430

in vec3 direc;
uniform samplerCube map;


void main()
{
	gl_FragColor = texture(map, direc);
	gl_FragColor = vec4(direc,1);
}