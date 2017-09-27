#version 430

layout (location = 5) uniform vec3 lightLoc;
layout (location = 6) uniform vec3 camLoc;

in vec3 fragLoc;
in vec3 fragNorm;
in vec2 UV;

uniform sampler2D myTexture;

void main()

{
	
	vec3 L = normalize(lightLoc - fragLoc); 
	vec3 E = normalize(camLoc - fragLoc);
	vec3 H = normalize(L + E);

	float Ambient = .35;
	float Diffuse = max(dot(fragNorm, L), 0.0);
	float Specular = pow(clamp(dot(fragNorm, H), 0.0, 1.0), 16);
	float Brightness = Ambient + Diffuse + Specular;
	vec4 Colour = texture(myTexture, UV) * Brightness;
	Colour.w = 1;
	gl_FragColor = Colour;

}