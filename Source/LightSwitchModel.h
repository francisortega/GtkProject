// LightSwitchModel.h
// Purpose: A model representing a light switch. This light switch implements the IPickable
//			interface and can therefore be interacted with through picking.
//
// @author Michael Lavoie

#pragma once

#include "Model.h"
#include "IPickable.h"

#include <GL/glew.h>
#include <vector>

class LightSwitchModel : public Model, public IPickable
{
public:
	LightSwitchModel(glm::vec3 &lightBulbColor, Model* parent = NULL);
	virtual ~LightSwitchModel();

	virtual void Picked();
	virtual std::vector<glm::vec3> GetWorldVertices();

	virtual void Update(float dt);
	virtual void Draw();
	bool isDoorOpen = false;

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:
	std::vector<glm::vec3> vertices;

	unsigned int numVertices;

	GLuint Texture;
	GLuint TextureID;

	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;
	unsigned int mNormalBufferID;
	unsigned int mUVBufferID;

	// Material Coefficients
	const float ka = 0.3f;
	const float kd = 0.5f;
	const float ks = 0.0f;
	const float n = 400.0f;

	const float tr = 1.0f;
	float noShading = 0.0f;

	// The light bulb the this switch controls
	glm::vec3* lightBulbColor;
};

