/**
 * Frame Color Model
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

class FrameColorModel : public Model
{
public:
	FrameColorModel(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f), Model* parent = NULL);
	virtual ~FrameColorModel();

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

	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;

	// Material Coefficients
	const float ka = 0.1f;
	const float kd = 0.9f;
	const float ks = 0.5f;
	const float n = 450.0f;

	const float tr = 1.0f;
	float noShading = 0.0f;

};
