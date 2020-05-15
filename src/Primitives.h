#pragma once
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include "Vertex.h"

class Primitive {
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;


public:

	Primitive() {

	}

	void init(const Vertex* vertices, const unsigned noVertices, const GLuint* indices, const unsigned noIndices) {
		for (size_t i = 0; i < noVertices; i++) {
			this->vertices.push_back(vertices[i]);
		}

		for (size_t i = 0; i < noIndices; i++) {
			this->indices.push_back(indices[i]);
		}
	}

	inline Vertex* getVertices() {
		return this->vertices.data();
	}

	inline GLuint* getIndices() {
		return this->indices.data(); 
	}

	const size_t getNoVertices() { return this->vertices.size(); }
	const size_t getNoIndices() { return this->indices.size(); }

	virtual ~Primitive() {

	}
};

class Triangle : public Primitive {
public:
	Triangle(): Primitive() {
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

		this->init(vertices, noVertices, indices, noIndices);
	}
};