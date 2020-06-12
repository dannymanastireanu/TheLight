#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm.hpp>

#include "Shader.h"


class Material {

private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLint diffuseTex;
	GLint specularTex;


public:
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLint diffuseTex, GLint specularTex) {
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->diffuseTex = diffuseTex;
		this->specularTex = specularTex;
	}

	~Material(){
	
	}

	void sendToShader(Shader &shader) {
		shader.setVec3f(this->ambient, "material.ambient");
		shader.setVec3f(this->diffuse, "material.diffuse");
		shader.setVec3f(this->specular, "material.specular");
		shader.set1i(this->diffuseTex, "material.textureImg");
		shader.set1i(this->specularTex, "material.normalTextureImg");
	}
};