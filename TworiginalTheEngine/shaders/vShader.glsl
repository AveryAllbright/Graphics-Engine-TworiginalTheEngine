#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

layout (location = 3) uniform mat4 modelWorld;
layout (location = 4) uniform mat4 cameraMat;

out vec3 fragLoc;
out vec3 fragNorm;
out vec2 UV;

void main()

{
	vec4 worldPosition = modelWorld * vec4(position,1);

	gl_Position = cameraMat * worldPosition;

	vec3 Vecnormal = vec3(transpose(inverse(modelWorld)) * vec4(normal, 0));
	
	fragNorm = Vecnormal;
	fragLoc = vec3(worldPosition);
	UV = uv;

}