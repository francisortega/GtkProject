// RubiksCubeModel.h
// Purpose: A model representing a static rubiks cube.
//
// @author Michael Lavoie

#pragma once

#include "Model.h"

#include <GL/glew.h>

class RubiksCubeModel : public Model
{
public:
	RubiksCubeModel(Model* parent = NULL);
	virtual ~RubiksCubeModel();

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
	const float ka = 0.1f;
	const float kd = 0.05f;
	const float ks = 0.0f;
	const float n = 400.0f;

	const float tr = 1.0f;
	float noShading = 0.0f;


};

