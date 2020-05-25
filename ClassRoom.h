#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>


// Class to minimize main stuff
class ClassRoom
{
private:

	GLFWwindow* window;

	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;
	int framebufferWidth, framebufferHeight;

	void setupWindow(const char* winName, const int width, const int height, int& frameBufferWidht, int& frameBufferHeight, const int glMin, const int glMaj);

public:
	ClassRoom();
	virtual ~ClassRoom();
};

