#include "Vertex.h"
#include "Primitives.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "EntryPoint.hpp"

const char* vfile = "res\\shaders\\vertex.glsl";
const char* ffile = "res\\shaders\\fragment.glsl";



void updateInput(GLFWwindow* window, Mesh& mesh) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		mesh.move(glm::vec3(0.0f, 0.0f, +0.02f));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		mesh.move(glm::vec3(0.0f, 0.0f, -0.02f));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		mesh.move(glm::vec3(0.02f, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		mesh.move(glm::vec3(-0.02f, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		mesh.rotate(glm::vec3(0.0f, 2.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		mesh.rotate(glm::vec3(0.0f, -2.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		mesh.rotate(glm::vec3(-2.0f, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		mesh.rotate(glm::vec3(2.0f, 0.0f,0.0f));
	}

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {

	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {

	}
}

void error_callback(int error, const char* description) {
	fprintf(stderr, "Error %d: %s\n", error, description);
}



Vertex vertices[] = {
	glm::vec3(0.0f, 0.5f, 0.0f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f),	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(-0.5f, -0.5f, 0.0f),		glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.5f, -0.5f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f)
};

unsigned int noVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] = {
	0, 1, 2
};

unsigned int noIndices = sizeof(indices) / sizeof(GLuint);


Triangle test;

entry_point{

	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;
	int framebufferWidth, framebufferHeight;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		return -1;
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
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

	//Some difficult stuff who doens't work
	/*glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);*/

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Shader init
	Shader mainShader(vfile, ffile);

	// Model mesh init
	Mesh mesh(test.getVertices(), test.getNoVertices(), test.getIndices(), test.getNoIndices());


	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	Texture wallTexture("res\\texture\\brickwall.jpg", GL_TEXTURE_2D, 0);
	Texture wallTextureNormal("res\\texture\\brickwall_normal.jpg", GL_TEXTURE_2D, 1);
	Material materialWall(glm::vec3(0.1f), glm::vec3(0.75f), glm::vec3(1.0f), wallTexture.getUnit(), wallTextureNormal.getUnit());


	//view position
	glm::vec3 lightPosition(0.0f, 0.0f, 0.2f);
	glm::vec3 viewPosition(glm::vec3(0, 0, 1));



	glm::mat4 viewMatrix = glm::lookAt(
		viewPosition, // Camera is at (x, x, x), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);


	float FoV = 55.0f;
	// Generates a really hard-to-read matrix, but a normal, standard 4x4 matrix nonetheless
	glm::mat4 projectionMatrix = glm::perspective(
		glm::radians(FoV),									// The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
		(float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,			// Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
		0.1f,												// Near clipping plane. Keep as big as possible, or you'll get precision issues.
		100.0f												// Far clipping plane. Keep as little as possible.
	);


	mainShader.use();

	mainShader.setMat4fv(viewMatrix, "viewMatrix");
	mainShader.setMat4fv(projectionMatrix, "projectionMatrix");
	mainShader.setVec3f(lightPosition, "lightPosition");
	mainShader.setVec3f(viewPosition, "viewPosition");


	mainShader.disable();

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		updateInput(window, mesh);


		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		mainShader.use();
		materialWall.sendToShader(mainShader);


		wallTexture.bind();

		mesh.render(&mainShader);


		glfwSwapBuffers(window);
		glFlush();

		glBindVertexArray(0);
		mainShader.disable();
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//End of program
	glfwDestroyWindow(window);
	glfwTerminate();
	glDeleteProgram(mainShader.getMainProgram());
	mainShader.~Shader();

	return 0;
}
