/**
 * Window Frame Model
 *
 * Header
 *
 * @author Francis Ortega
 * @author Nicholas Bergeron - Framework Creator
 * @version August 2014
 * @copyright (c) 2014 Concordia University. All rights reserved.
 */

#pragma once

#include "Model.h"

#include <GL/glew.h>

class WindowFrameModel : public Model
{
public:
	WindowFrameModel();
	virtual ~WindowFrameModel();

	virtual void Update(float dt);
	virtual void Draw();

	glm::vec4 GetPlaneEquation();
	std::vector<glm::vec2> GetWindowRange();

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:
	// The vertex format could be different for different types of models
	std::vector<Model*> mWindow;
	unsigned int numVertices;

	GLuint Texture;
	GLuint TextureID;

	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;
	unsigned int mNormalBufferID;
	unsigned int mUVBufferID;
	
	// Material Coefficients
	const float ka = 0.5f;
	const float kd = 0.9f;
	const float ks = 0.5f;
	const float n = 450.0f;

	std::vector<glm::vec3> windowVertices;
};

