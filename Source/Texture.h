// Texture.h
// Purpose: This class provides functions to load image textures.
//
// @author Michael Lavoie

#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>
#include <GLM/glm.hpp>

class Texture
{
public:
	Texture();
	virtual ~Texture();

	static GLuint Texture::loadDDS(const char * imagepath);
};

