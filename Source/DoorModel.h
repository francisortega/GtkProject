// DoorModel.h
// Purpose: A model representing the house door. This door implements the IPickable
//			interface and can therefore be interacted with through picking.
//
// @author Michael Lavoie

#pragma once

#include "Model.h"
#include "IPickable.h"

#include <GL/glew.h>
#include <vector>

class DoorModel : public Model, public IPickable
{
public:
	DoorModel(Model* parent = NULL);
	virtual ~DoorModel();

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

	// This variable is used because the transform information is only set after object instantiation.
	bool isFirstRun = true;

	float openedAngle;
	float closedAngle;
	float endAngle;

	// Config options
	float doorAnimationSpeed = 95.0f;
	float maxDoorOpenAngle = 90.0f;
};

