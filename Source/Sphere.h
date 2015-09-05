//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 22/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.
//

#pragma once

#include "Model.h"
#include <glm/glm.hpp>

#include <GL/glew.h>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
};

//Vertex* GetRedSphereVertexBuffer(unsigned int &numVertices);

class Sphere : public Model
{
public:
	Sphere(int sphereType, glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f), Model* parentModel = NULL);
	~Sphere();
	virtual void Update(float dt);
	virtual void Draw();

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:

	unsigned int numVertices;

	GLuint Texture;
	GLuint TextureID;

	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;
	unsigned int mNormalBufferID;
	unsigned int mUVBufferID;

	// Material Coefficients
	const float ka = 0.9f;
	const float kd = 0.9f;
	const float ks = 0.75f;
	const float n = 450.0f;

	const float tr = 1.0f;
	float noShading = 0.0f;
};
