// Skybox.h
// Purpose: A model representing the skybox around the scene

// @author Philippe Thibault
#pragma once

#include "Model.h"

#include <GL/glew.h>

class Skybox : public Model
{
public:
	Skybox(int type, Model* parent = NULL);
	virtual ~Skybox();

	virtual void Update(float dt);
	virtual void Draw();
	virtual void SetTransparency(float trans);

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

	GLuint TextureOne;
	GLuint TextureIdOne;
	GLuint TextureTwo;
	GLuint TextureIdTwo;
	GLuint TextureThree;
	GLuint TextureIdThree;
	GLuint TextureFour;
	GLuint TextureIdFour;

	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;
	unsigned int mNormalBufferID;
	unsigned int mUVBufferID;

	// Material Coefficients
	const float ka = 0.6f;
	const float kd = 0.9f;
	const float ks = 0.5f;
	const float n = 450.0f;

	float tr = 1.0;
	float noShading = 1.0;

	int type;
};

