#pragma once

#include <vector>

#include "Vertex.h"
#include "Primitives.h"
#include "Texture.h"
#include "Material.h"


class Mesh {
private:
	unsigned noVertices;
	unsigned noIndices;

	GLuint VAO, VBO, EBO;
	glm::vec3 rotation, scale, position;
	glm::mat4 modelMatrix;

	void setupMesh(Primitive* primitives) {

		this->noVertices = primitives->getNoVertices();
		this->noIndices = primitives->getNoIndices();

		// Create VAO
		glCreateVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Trimitem data in bytes catre gpu
		glBufferData(GL_ARRAY_BUFFER, this->noVertices * sizeof(Vertex), primitives->getVertices(), GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->noIndices * sizeof(GLuint), primitives->getIndices(), GL_STATIC_DRAW);

		//Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		//Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);
		//Texture coord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);
		//Normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);


		glBindVertexArray(0);
	}

	void setupMesh(Vertex* vertices, const unsigned& noVertices, GLuint* indices, const unsigned& noIndices) {

		this->noVertices = noVertices;
		this->noIndices = noIndices;

		// Create VAO
		glCreateVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Trimitem data in bytes catre gpu
		glBufferData(GL_ARRAY_BUFFER, this->noVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->noIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);

		//Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		//Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);
		//Texture coord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);
		//Normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);


		glBindVertexArray(0);
	}

	void updateUniforms(Shader* shader) {
		shader->setMat4fv(modelMatrix, "modelMatrix");
	}

	void updateModelMatrix() {
		this->modelMatrix = glm::mat4(1.0f);
		this->modelMatrix = glm::translate(modelMatrix, position);
		this->modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		this->modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		this->modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		this->modelMatrix = glm::scale(modelMatrix, scale);
	}

public:

	Mesh(Primitive* primitive,
		glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f)) {

		this->position = position;
		this->rotation = rotation;
		this->scale = scale;

		this->setupMesh(primitive);
		this->updateModelMatrix();
	}

	Mesh(Vertex* vertexArray, const unsigned& noVertices, GLuint* indexArray, const unsigned& noIndices,
		glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f)) {

		this->position = position;
		this->rotation = rotation;
		this->scale = scale;

		this->setupMesh(vertexArray, noVertices, indexArray, noIndices);
		this->updateModelMatrix();
	}

	void update();
	void render(Shader* shaderProgram) {

		this->updateModelMatrix();
		this->updateUniforms(shaderProgram);
		
		shaderProgram->use();

		glBindVertexArray(VAO);
		// Render
		glDrawElements(GL_TRIANGLES, noIndices, GL_UNSIGNED_INT, 0);

	}

	void setPosition(const glm::vec3 position) {
		this->position = position;
	}

	void setRotation(const glm::vec3 rotation) {
		this->rotation = rotation;
	}

	void setScale(const glm::vec3 scale) {
		this->scale = scale;
	}

	// Action

	void move(const glm::vec3 position) {
		this->position += position;
	}

	void rotate(const glm::vec3 rotation) {
		this->rotation += rotation;
	}

	void scaleUp(const glm::vec3 scale) {
		this->scale += scale;
	}



	~Mesh() {
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->EBO);
	}
};