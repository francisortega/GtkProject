// Grass.h
// Purpose: A model representing the grass in the scene
//
// @author Philippe Thibault
#pragma once

#include "Model.h"

#include <GL/glew.h>

class Grass : public Model
{
public:
	Grass(Model* parent = NULL);
	virtual ~Grass();

	virtual void Update(float dt);
	virtual void Draw();

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:
	// The vertex format could be different for different types of models
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
	};

	unsigned int numVertices;

	GLuint Texture;
	GLuint TextureID;

	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;
	unsigned int mNormalBufferID;
	unsigned int mUVBufferID;

	// Material Coefficients
	const float ka = 0.6f;
	const float kd = 0.9f;
	const float ks = 0.5f;
	const float n = 450.0f;

	const float tr = 1.0f;
	float noShading = 0.0f;

};

