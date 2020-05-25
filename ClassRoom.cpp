#include "ClassRoom.h"


void ClassRoom::setupWindow(const char* winName, const int width, const int height, int& frameBufferWidht, int& frameBufferHeight, const int glMin, const int glMaj) {
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMaj);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMin);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(width, height, winName, NULL, NULL);
	glfwGetFramebufferSize(window, &frameBufferWidht, &frameBufferHeight);
	glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(window);
}

ClassRoom::ClassRoom() {
}

ClassRoom::~ClassRoom()
{
}
