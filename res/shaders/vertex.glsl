#version 440 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texcoord;


out vec3 vsPosition;
out vec3 vsColor;
out vec2 vsTexcoord;


void main() {

	vsPosition = position;
	vsColor = color;
	vsTexcoord = vec2(texcoord.x, texcoord.y);
	
	gl_Position = vec4(position, 1.0f);
}