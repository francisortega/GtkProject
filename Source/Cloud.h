// Cloud.h
// Purpose: A model representing the particle system for a cloud
//
// @author Philippe Thibault
#pragma once

#include "Model.h"
#include <glm/glm.hpp>

#include <GL/glew.h>

class Cloud : public Model
{
public:
	Cloud(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f), Model* parentModel = NULL);
	~Cloud();
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
