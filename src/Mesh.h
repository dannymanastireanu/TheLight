#pragma once

#include <vector>

#include "Vertex.h"
#include "Texture.h"
#include "Material.h"


class Mesh {
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	GLuint VAO, VBO, EBO;
	glm::vec3 rotation, scale, position;
	glm::mat4 modelMatrix;

	void init(Vertex* vertexArray, const unsigned& noVertices, GLuint* indexArray, const unsigned& noIndices) {
		for (size_t i = 0; i < noVertices; i++)
		{
			this->vertices.push_back(vertexArray[i]);
		}

		for (size_t i = 0; i < noIndices; i++)
		{
			this->indices.push_back(indexArray[i]);
		}
	}

	void initVAO() {
		glCreateVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Trimitem data in bytes catre gpu
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);


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
	}

	void initModelMatrix() {

		this->position = glm::vec3(0.0f);
		this->rotation = glm::vec3(0.0f);
		this->scale	   = glm::vec3(1.0f);

		this->modelMatrix = glm::mat4(1.0f);
		this->modelMatrix = glm::translate(modelMatrix, position);
		this->modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		this->modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		this->modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		this->modelMatrix = glm::scale(modelMatrix, scale);
	}

	void updateUniforms(Shader* shader) {
		shader->setMat4fv(modelMatrix, "modelMatrix");
	}

public:

	Mesh(Vertex* vertexArray, const unsigned& noVertices, GLuint* indexArray, const unsigned& noIndices) {
		this->init(vertexArray, noVertices, indexArray, noIndices);
		this->initVAO();
		this->initModelMatrix();
	}

	void update();
	void render(Shader* shaderProgram) {

		this->updateUniforms(shaderProgram);
		
		shaderProgram->use();

		glBindVertexArray(VAO);
		// Render
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

	}

	~Mesh() {
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->EBO);
	}
};