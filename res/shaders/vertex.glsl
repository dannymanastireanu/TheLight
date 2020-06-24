#version 440 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texcoord;
layout (location = 3) in vec3 normal;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent; 


out vec3 vsPosition;
out vec3 vsColor;
out vec2 vsTexcoord;
out vec3 vsNormal;
out mat3 vsTBN;

uniform mat4 MVP;
uniform mat4 modelMatrix, viewMatrix, projectionMatrix;
uniform mat4 lightSpaceMatrix;

out mat4 vsModelMatrix;
out vec4 FragPosLightSpace;

void main() {

	vsPosition = vec4(modelMatrix * vec4(position, 1.0f)).xyz;
	vsColor = color;
	vsTexcoord = vec2(texcoord.x, texcoord.y);

	// De ce transpuse and inverse?
	vsNormal = mat3(transpose(inverse(modelMatrix))) * normal;
	
	vsModelMatrix = modelMatrix;

	//BEGIN tag and bitag calculation

	vec3 T = normalize(vec3(modelMatrix * vec4(aTangent,   0.0)));
//	vec3 B = normalize(vec3(modelMatrix * vec4(aBitangent, 0.0)));
	vec3 N = normalize(vec3(modelMatrix * vec4(normal,    0.0)));
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	vec3 B = cross(N, T);
	mat3 TBN = mat3(T, B, N);
	vsTBN = TBN;

	
	//END tag and bitag calculation

	// Shadow-mapping
	FragPosLightSpace = lightSpaceMatrix * vec4(vsPosition, 1.0);
	
	gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0f);
}