#pragma once

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture {
private:
	GLuint texture;
	int width, height;
	GLuint textureUnit, type;

public:
	Texture(const char* fn, GLenum type, GLuint textureUnit) {

		this->textureUnit = textureUnit;
		this->type = type;

		unsigned char* image = stbi_load(fn, &width, &height, NULL, 0);

		glGenTextures(1, &texture);
		glBindTexture(type, texture);

		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (image) {
			glTexImage2D(type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(type);
		}
		else {
			std::cout << "Error load texture with stbi_load with file: " << fn << std::endl;
		}

		glActiveTexture(0);
		glBindTexture(type, 0);
		stbi_image_free(image);
	}

	inline GLuint getUnit() const { return textureUnit; }

	inline GLuint getId() const { return texture;}

	void bind() {
		glActiveTexture(GL_TEXTURE0 + this->textureUnit);
		glBindTexture(this->type, this->texture);
	}

	void unbind(GLenum type) {
		glActiveTexture(0);
		glBindTexture(type, 0);
	}

	void load(const char* fn) {

		if (this->texture)
			glDeleteTextures(1, &this->texture);

		unsigned char* image = stbi_load(fn, &width, &height, NULL, 0);

		glGenTextures(1, &texture);
		glBindTexture(type, texture);

		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (image) {
			glTexImage2D(type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(type);
		}
		else {
			std::cout << "Error load texture with stbi_load with file: " << fn << std::endl;
		}

		glActiveTexture(0);
		glBindTexture(type, 0);
		stbi_image_free(image);
	}

	~Texture() {
		glDeleteTextures(1, &texture);
	}
};