#include "Shader.h"
#include "EntryPoint.hpp"
//I will change this bullshit later.
const char* vfile = "D:\\A.C\\SPG\\Project\\TheLight\\res\\shaders\\vertex.glsl";
const char* ffile = "D:\\A.C\\SPG\\Project\\TheLight\\res\\shaders\\fragment.glsl";


void updateInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void updateInput(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position.z += 0.03f;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position.z -= 0.03f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position.x += 0.03f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position.x -= 0.03f;
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		rotation.z -= 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		rotation.z += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		rotation.x -= 3.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		rotation.x += 3.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		rotation.y -= 2.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		rotation.y += 2.0f;
	}
}

void error_callback(int error, const char* description) {
	fprintf(stderr, "Error %d: %s\n", error, description);
}



Vertex vertices[] = {
	glm::vec3(0.0f, 0.5f, -1.0f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f),	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(-0.5f, -0.5f, -1.0f),		glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.5f, -0.5f, -1.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f)
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


	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

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
	//Normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);

	
	glBindVertexArray(0);

	//Do some stuff with texture
	int imageHeight, nrChannels, imageWidth;
	unsigned char* image = stbi_load("D:\\A.C\\SPG\\Project\\TheLight\\res\\texture\\brickwall.jpg", &imageWidth, &imageHeight, &nrChannels, 0);

	GLuint wallTexture = 0;
	glGenTextures(1, &wallTexture);
	glBindTexture(GL_TEXTURE_2D, wallTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Error load texture with stbi_load." << std::endl;
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(image);

	// INIT MATRICES
	glm::vec3 position(0.0f);
	glm::vec3 rotation(0.0f);
	glm::vec3 scale(1.0f);

	//view position



	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, scale);


	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 1.0f), // Camera is at (x, x, x), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	
	float FoV = 45.0f;
	// Generates a really hard-to-read matrix, but a normal, standard 4x4 matrix nonetheless
	glm::mat4 projectionMatrix = glm::perspective(
		glm::radians(FoV),									// The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
		(float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,			// Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
		0.1f,												// Near clipping plane. Keep as big as possible, or you'll get precision issues.
		100.0f												// Far clipping plane. Keep as little as possible.
	);

	glm::mat4 MVPmatrix = projectionMatrix * viewMatrix * modelMatrix;

	glm::vec3 lightPosition(0.0f, 0.0f, 0.2f);
	glm::vec3 viewPosition(glm::vec3(0, 0, 1));

	mainShader.use();

	glUniformMatrix4fv(glGetUniformLocation(mainShader.getMainProgram(), "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(mainShader.getMainProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVPmatrix));
	glUniform3fv(glGetUniformLocation(mainShader.getMainProgram(), "lightPosition"), 1, glm::value_ptr(lightPosition));
	glUniform3fv(glGetUniformLocation(mainShader.getMainProgram(), "viewPosition"), 1, glm::value_ptr(viewPosition));

	mainShader.disable();

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		updateInput(window, position, rotation, scale);

		updateInput(window);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		mainShader.use();


		//Update uniform
		glUniform1i(glGetUniformLocation(mainShader.getMainProgram(), "wallTexture"), 0);
		
		modelMatrix = glm::mat4(1.0f);

		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::scale(modelMatrix, scale);
	
		glm::mat4 MVPmatrix = projectionMatrix * viewMatrix * modelMatrix;
		//glm::mat4 MVPmatrix = projectionMatrix * modelMatrix;
		
		glUniformMatrix4fv(glGetUniformLocation(mainShader.getMainProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVPmatrix));


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, wallTexture);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, noIndices, GL_UNSIGNED_INT, 0);

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
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(mainShader.getMainProgram());
	mainShader.~Shader();

	return 0;
}
