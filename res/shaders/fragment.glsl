#version 440 core


in vec3 vsPosition;
in vec3 vsColor;
in vec2 vsTexcoord;

out vec4 fragColor;

void main() {

	//Some problem with vsColor
	fragColor = vec4(vsPosition, 1.0f);
}
