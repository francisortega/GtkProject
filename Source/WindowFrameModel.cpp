/**
 * Window Frame Model
 *
 * Class - instantiates the classes Frame Color and Transparent to build a window
 *
 * @author Francis Ortega
 * @author Nicholas Bergeron - Framework Creator
 * @version August 2014
 * @copyright (c) 2014 Concordia University. All rights reserved.
 */

#include "WindowFrameModel.h"
#include "TransparentModel.h"
#include "FrameColorModel.h"

#include "Renderer.h"
#include "Texture.h"


// Include GLEW - OpenGL Extension Wrangler
#define GLEW_STATIC
#include <GL/glew.h>

using namespace glm;

WindowFrameModel::WindowFrameModel()
{
	mWindow.push_back(new FrameColorModel(glm::vec3(2.735, 0.52, 0.05), this)); // Top Frame
	mWindow.push_back(new FrameColorModel(glm::vec3(2.735, 0.52, 0.05), this)); // Bottom Frame
	mWindow.push_back(new FrameColorModel(glm::vec3(1.02, 1.035, 0.05), this)); // Left Frame
	mWindow.push_back(new FrameColorModel(glm::vec3(1.02, 1.035, 0.05), this)); // Right Frame
	mWindow.push_back(new TransparentModel(glm::vec3(0.7, 1.0, 0.02), this)); // Glass; last due to transparency

	// Position Frames
	mWindow[0]->SetPosition(glm::vec3(0.0, 0.71, 0.0));
	mWindow[1]->SetPosition(glm::vec3(0.0, -0.71, 0.0));
	mWindow[2]->SetPosition(glm::vec3(0.8575, 0.0, 0.0));
	mWindow[3]->SetPosition(glm::vec3(-0.8575, 0.0, 0.0));
}


WindowFrameModel::~WindowFrameModel()
{
}

void WindowFrameModel::Update(float dt)
{

}

void WindowFrameModel::Draw()
{
	for (std::vector<Model*>::iterator it = mWindow.begin(); it != mWindow.end(); ++it)
	{
		(*it)->Draw();
	}
}

bool WindowFrameModel::ParseLine(const std::vector<ci_string> &token)
{
	if (token.empty())
	{
		return true;
	}
	else
	{
		return Model::ParseLine(token);
	}
}

/**
 * GetPlaneEquation
 *
 * Get the plane equation of the transparent (glass) model and transform it to world space
 *
 * @return vec4 transformed normal vector in world space
 */
glm::vec4 WindowFrameModel::GetPlaneEquation()
{
	return inverse(transpose(GetWorldMatrix())) * dynamic_cast<TransparentModel*>(mWindow[4])->GetPlaneEquation();
}

/**
 * GetWindowRange
 *
 * Get the range (boundaries) of the transparent (glass) model in world space
 *
 * @return vector of ranges (boundaries)
 */
std::vector<glm::vec2> WindowFrameModel::GetWindowRange()
{
	return dynamic_cast<TransparentModel*>(mWindow[4])->GetBoundaries();
}
