#pragma once
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include "Vertex.h"

#include <iostream>
#include <fstream>
#include <string>

#define VERTEX "v"
#define TEXTURE "vt"
#define NORMAL "vn"
#define FACE "f"

#define VERTICES_PER_FACE 3

#define _CRT_SECURE_NO_WARNINGS

enum ObjectOption {Pyramid = 0, Sun, Porsche, Table};

class Primitive {
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;


public:

	Primitive() {

	}

	void init(const Vertex* vertices, const unsigned noVertices, const GLuint* indices, const unsigned noIndices) {

		// Aceasta linie pune manual vertices
		for (size_t i = 0; i < noVertices; i++) {
			this->vertices.push_back(vertices[i]);
		}

		for (size_t i = 0; i < noIndices; i++) {
			this->indices.push_back(indices[i]);
		}

		// Incarc din fisier vertices
		//this->loadDataFromObj("res\\shaders\\myPyramid.obj");
	}

	inline Vertex* getVertices() {
		return this->vertices.data();
	}

	inline GLuint* getIndices() {
		return this->indices.data(); 
	}

	const size_t getNoVertices() { return this->vertices.size(); }
	const size_t getNoIndices() { return this->indices.size(); }

	std::vector<std::string> split(const std::string& str, const std::string& delim)
	{
		std::vector<std::string> tokens;
		size_t prev = 0, pos = 0;
		do
		{
			pos = str.find(delim, prev);
			if (pos == std::string::npos) pos = str.length();
			std::string token = str.substr(prev, pos - prev);
			if (!token.empty()) tokens.push_back(token);
			prev = pos + delim.length();
		} while (pos < str.length() && prev < str.length());
		return tokens;
	}

	void loadDataFromObj(const char* fn) {
		std::vector<glm::vec3> vertexPosition;
		std::vector<glm::vec2> vertexTexture;
		std::vector<glm::vec3> vertexNormal;
		int indexDraw = 0;

		std::ifstream file(fn);
		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				if (line.rfind(VERTEX, 0) == 0 && line.rfind(TEXTURE, 0) != 0 && line.rfind(NORMAL, 0) != 0) {
					glm::vec3 v;
					sscanf_s(line.c_str(), VERTEX " %f %f %f\n", &v.x, &v.y, &v.z);
					vertexPosition.push_back(v);
					//printf("Vertex on %s\n", line.c_str());
				}
				else if (line.rfind(TEXTURE, 0) == 0) {
					glm::vec2 vt;
					sscanf_s(line.c_str(), TEXTURE " %f %f %f\n", &vt.x, &vt.y);
					vertexTexture.push_back(vt);
					//printf("TEXTURE on %s\n", line.c_str());
				}
				else if (line.rfind(NORMAL, 0) == 0) {
					glm::vec3 vn;
					sscanf_s(line.c_str(), NORMAL " %f %f %f\n", &vn.x, &vn.y, &vn.z);
					vertexNormal.push_back(vn);
					//printf("NORMAL on %s\n", line.c_str());
				}
				else if (line.rfind(FACE, 0) == 0) {

					std::vector<std::string> splitLine = split(line, " ");
					std::vector<std::vector<uint16_t>> indici;

					for (size_t i = 1; i <= VERTICES_PER_FACE; i++) {
						std::vector<std::string> temp = split(splitLine[i], "/");
						std::vector<uint16_t> vertexIdx;
						for (auto t : temp) {
							vertexIdx.push_back(stoi(t));
						}
						indici.push_back(vertexIdx);
					}

					glm::vec3 tangent, bitangent;
					for (size_t i = 0; i < VERTICES_PER_FACE; i++) {
						
						// BEGIN Compute tan && bitan

						if (i == 0) {
							glm::vec3 edge1 = vertexPosition[indici[1][0] - 1] - vertexPosition[indici[0][0] - 1];
							glm::vec3 edge2 = vertexPosition[indici[2][0] - 1] - vertexPosition[indici[1][0] - 1];

							glm::vec2 deltaUV1 = vertexTexture[indici[1][1] - 1] - vertexTexture[indici[0][1] - 1];
							glm::vec2 deltaUV2 = vertexTexture[indici[2][1] - 1] - vertexTexture[indici[0][1] - 1];

							float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

							tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
							tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
							tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
							tangent = glm::normalize(tangent);

							bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
							bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
							bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
							bitangent = glm::normalize(bitangent);
						}

						// END

						std::vector<uint16_t> vertex = indici[i];
						Vertex myVertex;

						myVertex.position = vertexPosition[vertex[0] - 1];
						myVertex.color = glm::vec3(0.0f, 0.0f, 0.0f);
						myVertex.texcoord = vertexTexture[vertex[1] - 1];
						myVertex.normal = vertexNormal[vertex[2] - 1];
						myVertex.tangent = tangent;
						myVertex.bitangen = bitangent;
						vertices.push_back(myVertex);
					
						indices.push_back(indexDraw++); // Punem indicii dupa care sa deseneze
					}
				}
			}
			file.close();
		}
	}

	virtual ~Primitive() {

	}
};

class Triangle : public Primitive {
public:
	Triangle(): Primitive() {
		Vertex vertices[] = {
			// Triangle with tan and bitan manually calculated
			glm::vec3(0.0f, 0.5f, 0.0f),		glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec2(0.0f, 1.0f),	glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.447213f, 0.89442f, 0.0f),
			glm::vec3(-0.5f, -0.5f, 0.0f),		glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec2(0.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.447213f, 0.89442f, 0.0f),
			glm::vec3(0.5f, -0.5f, 0.0f),		glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec2(1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.447213f, 0.89442f, 0.0f)
		};

		unsigned int noVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] = {
			0, 1, 2
		};

		unsigned int noIndices = sizeof(indices) / sizeof(GLuint);

		this->init(vertices, noVertices, indices, noIndices);
	}

	// Just for test to see how it's look tan and bitan
	void tryTagBitag() {

		glm::vec3 tangent1, bitangent1;

		glm::vec3 edge1 = this->getVertices()[1].position - this->getVertices()[0].position;
		glm::vec3 edge2 = this->getVertices()[2].position - this->getVertices()[0].position;

		glm::vec2 deltaUV1 = this->getVertices()[1].texcoord - this->getVertices()[0].texcoord;
		glm::vec2 deltaUV2 = this->getVertices()[2].texcoord - this->getVertices()[0].texcoord;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = glm::normalize(tangent1);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = glm::normalize(bitangent1);
	}
};

class Background : public Primitive {
public:
	Background() : Primitive() {
		Vertex vertices[] = {
			glm::vec3(-1.0f, 1.0f, 0.0f),		glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec2(0.0f, 1.0f),	glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.447213f, 0.89442f, 0.0f),
			glm::vec3(1.0f, 1.0f, 0.0f),		glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec2(0.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.447213f, 0.89442f, 0.0f),
			glm::vec3(1.0f, -1.0f, 0.0f),		glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec2(1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.447213f, 0.89442f, 0.0f),
			glm::vec3(-1.0f, -1.0f, 0.0f),		glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec2(1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.447213f, 0.89442f, 0.0f)
		};

		unsigned int noVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] = {
			0, 1, 2, 0, 2, 3
		};

		unsigned int noIndices = sizeof(indices) / sizeof(GLuint);

		this->init(vertices, noVertices, indices, noIndices);
	}
};


class ComplexObject : public Primitive {
public:
	ComplexObject(int type) : Primitive() {
		switch (type)
		{
			case Pyramid:
				this->loadDataFromObj("res\\shaders\\pyramid\\myPyramid.obj");
				break;
			case Sun:
				this->loadDataFromObj("res\\shaders\\sun\\sun.obj");
				break;
			case Porsche:
				this->loadDataFromObj("res\\shaders\\porsche\\porsche.obj");
				break;
			case Table:
				this->loadDataFromObj("res\\shaders\\table\\table.obj");
				break;
			default:
				throw "Wrong obj";
				break;
		}
	};
};