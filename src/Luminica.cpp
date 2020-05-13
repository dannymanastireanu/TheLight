#include "Shader.h"
#include "EntryPoint.hpp"
//I will change this bullshit later.
const char* vfile = "D:\\Downloads\\Github\\Luminica\\res\\shaders\\vertex.glsl";
const char* ffile = "D:\\Downloads\\Github\\Luminica\\res\\shaders\\fragment.glsl";


void updateInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void error_callback(int error, const char* description) {
	fprintf(stderr, "Error %d: %s\n", error, description);
}



Vertex vertices[] = {
	glm::vec3(0.0f, 0.5f, 0.0f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f),
	glm::vec3(-0.5f, -0.5f, 0.0f),		glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f),
	glm::vec3(0.5f, -0.5f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(1.0f, 0.0f)
};

unsigned int noVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] = {
	0, 1, 2
};

unsigned int noIndices = sizeof(indices) / sizeof(GLuint);

entry_point{

	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;
	int framebufferWidth, framebufferHeight;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		return -1;
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenRender", NULL, NULL);
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(window);

	GLint err;
	if ((err = glewInit()) != GLEW_OK) {
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cerr << glewGetErrorString(err);
		glfwTerminate();
		return -1;
	}

	//Some difficult stuff
	/*glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

	Shader mainShader(vfile, ffile);
	mainShader.use();


	//VAO, VBO, EBO
	GLuint VAO = -1;
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO = -1;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint EBO = -1;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	
	glBindVertexArray(0);

	//Do some stuff with texture
	int imageHeight, nrChannels, imageWidth;
	unsigned char* image = stbi_load("brickwall.jpg", &imageWidth, &imageHeight, &nrChannels, 0);


	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		updateInput(window);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		mainShader.use();
		glBindVertexArray(VAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, noIndices, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glFlush();
	}

	//End of program
	glfwDestroyWindow(window);
	glfwTerminate();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(mainShader.getMainProgram());
	mainShader.~Shader();

	return 0;
}
