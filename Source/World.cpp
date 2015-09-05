/**
 * World
 *
 * @author Philippe Thibault
 *			- added alpha algorithm for skyboxes to seamlessly switch between each other
 *			- multiple light sources for the sun and moon
 *			- rotate sun and moon as well as their light sources around the origin
 *          - interpolate colors as the sun and moon move around the scene
 *
 * @author Francis Ortega
 *			- contributed to multiple light sources and added the light bulb source and color
 *			- created the window model for the house
 *			- passed house and window boundaries and window plane to the fragment shader
 */

//
// COMP 371 Project Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.
//

#include "World.h"
#include "Renderer.h"
#include "ParsingHelper.h"
#include "Skybox.h"
#include "Cloud.h"

#include "StaticCamera.h"
#include "FirstPersonCamera.h"

#include "CubeModel.h"
#include "RubiksCubeModel.h"
#include "LightBulbModel.h"
#include "WindowFrameModel.h"
#include "Sphere.h"
#include "Sun.h"
#include "Moon.h"
#include "Grass.h"
#include "DoorModel.h"
#include "CrosshairModel.h"
#include "TransparentModel.h"
#include "SofaModel.h"
#include "HouseModel.h"
#include "TableModel.h"
#include "LightSwitchModel.h"

#include "Picker.h"

#include <GLFW/glfw3.h>
#include "EventManager.h"
#include <GLM/glm.hpp>
#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;
using namespace glm;

int sunIndex, moonIndex;
int sunlightIndex, moonlightIndex, currSkybox;
static float rotationAngle = 0, transparency;
float rotationTracker = 0;
int intRotationTracker, lastIntRotationTracker;

bool   gp, ascendingSkybox = true;                      // G Pressed? ( New )
GLuint filter;                      // Which Filter To Use
GLuint fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR };   // Storage For Three Types Of Fog
GLuint fogfilter = 1;                    // Which Fog To Use
GLfloat fogColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };      // Fog Color

World::World()
{
	// Setup Camera
	mCamera.push_back( new StaticCamera( vec3(1.0f, 2.0f, 3.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f) ) );
	mCamera.push_back( new FirstPersonCamera( vec3(0.5f, 0.5f, 5.0f), this) );
	mCurrentCamera = 0;
	// The geometry should be loaded from a scene file

	mModel.push_back(new CrosshairModel(mCamera[1]));

	transparency = 1.0;
}

World::~World()
{
	// Models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		delete *it;
	}

	mModel.clear();

	// Camera
	for (vector<Camera*>::iterator it = mCamera.begin(); it < mCamera.end(); ++it)
	{
		delete *it;
	}
	mCamera.clear();
}
	
void World::Update(float dt)
{
	// User Inputs
	// 0 1 2 to change the Camera
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_1 ) == GLFW_PRESS)
	{
		mCurrentCamera = 0;
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_2 ) == GLFW_PRESS)
	{
		if (mCamera.size() > 1)
		{
			mCurrentCamera = 1;
		}
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_3 ) == GLFW_PRESS)
	{
		if (mCamera.size() > 2)
		{
			mCurrentCamera = 2;
		}
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_F) == GLFW_PRESS)
	{
		unsigned int currShader = Renderer::GetCurrentShader();
		if (currShader == 0) {
			Renderer::SetShader(SHADER_FOG);
		}
		else {
			Renderer::SetShader(SHADER_PHONG);
		}
	}

	
	if (glfwGetMouseButton(EventManager::GetWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		mouseClickEnable = true;
	}
	else if (glfwGetMouseButton(EventManager::GetWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && mouseClickEnable)
	{
		mouseClickEnable = false;

		int W, H;

		if (EventManager::IsMouseCursorEnabled())
		{
			Picker::getInstance().pick(EventManager::GetMousePositionX(), EventManager::GetMousePositionY(), mCamera[mCurrentCamera]);
		}
		else
		{
			glfwGetWindowSize(EventManager::GetWindow(), &W, &H);
			Picker::getInstance().pick((float)W / 2, (float)H / 2, mCamera[mCurrentCamera]);
		}
	}

	// Update current Camera
	mCamera[mCurrentCamera]->Update(dt);

	rotationAngle = 0.25;
#pragma region rotationlogic - Philippe
	vec3 rotateAboutThisPoint = vec3(0, 0, 0); // The origin of rotation
	vec3 rotateAboutThisAxis = vec3(0, 0, 1); // The axis of rotation

	mat4 translateToPoint = translate(mat4(1.0f), rotateAboutThisPoint);
	mat4 rotateAroundAxis = rotate(mat4(1.0f), rotationAngle, rotateAboutThisAxis);
	mat4 reverseTranslation = translate(glm::mat4(1.0f), -rotateAboutThisPoint);

	mat4 matrixTransform = translateToPoint * rotateAroundAxis * reverseTranslation;
	//rotate the sun
	mModel[sunIndex]->SetPosition(vec3(matrixTransform * vec4(mModel[sunIndex]->GetPosition(), 1)));
	//rotate the moon
	mModel[moonIndex]->SetPosition(vec3(matrixTransform * vec4(mModel[moonIndex]->GetPosition(), 1)));
#pragma endregion rotationlogic - Philippe

#pragma region alternaterotationlogic - Philippe
	/* ALTERNATE ROTATION (only rotates around a vector)
	mModel[sunIndex]->SetPosition(
	glm::vec3(
	glm::rotate(
	glm::mat4(1.0f),
	rotationAngle,
	glm::vec3(1, 0, 0)
	)
	*
	glm::vec4(
	mModel[sunIndex]->GetPosition(),
	1)
	)
	);

	mModel[moonIndex]->SetPosition(
	glm::vec3(
	glm::rotate(
	glm::mat4(1.0f),
	rotationAngle,
	glm::vec3(1, 0, 0)
	)
	*
	glm::vec4(
	mModel[moonIndex]->GetPosition(),
	1)
	)
	);
	*/
#pragma endregion

#pragma region skyboxlogic
	if (skyboxes.size() > 1) {
		if ((transparency != 0 && ascendingSkybox) || (transparency != 1 && !ascendingSkybox)) {
			if (ascendingSkybox) {
				transparency -= 0.001;
			}
			else {
				transparency += 0.001;
			}
			if (transparency < 0) {
				transparency = 0;
				skyboxes[currSkybox]->SetTransparency(0);
			}
			else if (transparency > 1) {
				transparency = 1;
				skyboxes[currSkybox]->SetTransparency(1);
			}
			else {
				skyboxes[currSkybox]->SetTransparency(transparency);
			}
		}
		else {
			//cout << "CURRENT SKYBOX :" << currSkybox << endl;
			//cout << "TRANSPARENCY :" << transparency << endl;
			if (ascendingSkybox) {
				//cout << "ASCENDING" << endl;
			}
			else {
				//cout << "DESCENDING" << endl;
			}

			if (currSkybox == 0) {
				if (!ascendingSkybox) {
					++currSkybox;
				}
				ascendingSkybox = false;
				transparency = 0.0;
			}
			else if (currSkybox == skyboxes.size() - 1) {
				if (ascendingSkybox) {
					--currSkybox;
				}
				ascendingSkybox = true;
				transparency = 1.0;			
			}
			else {
				if (ascendingSkybox) {
					--currSkybox;
					transparency = 1.0;
				}
				else {
					++currSkybox;
					transparency = 0.0;
				}
			}
		}
	}
#pragma endregion skyboxlogic - Philippe

	//WORK WITH LIGHTS
	sunlightPosition = vec4(vec3(mModel[sunIndex]->GetPosition().x, mModel[sunIndex]->GetPosition().y, mModel[sunIndex]->GetPosition().z), 0.0);
	moonlightPosition = vec4(vec3(mModel[moonIndex]->GetPosition().x, mModel[moonIndex]->GetPosition().y, mModel[moonIndex]->GetPosition().z), 0.0);
	
#pragma region colorlogic 
	rotationTracker += (rotationAngle);
	intRotationTracker = rotationTracker;
	lastIntRotationTracker = intRotationTracker - 1;

	//cout << intRotationTracker << endl;
	//cout << lastIntRotationTracker << endl;

	if (intRotationTracker >= 360) {
		rotationTracker = 0;

    	currSunColor = SunColorA;
		currMoonColor = MoonColorA;
	}
	else if (intRotationTracker > lastIntRotationTracker) {
		lastIntRotationTracker = intRotationTracker;
		//sun high
		if (rotationTracker >= 0 && rotationTracker < 30 || rotationTracker >= 330 && rotationTracker < 360) {
			bool rising = true;
			if (rotationTracker >= 330 && rotationTracker < 360) {
				rising = false;
			}
			float colorShiftRPerAngle = (SunColorA.x - SunColorB.x) / 30.0f;
			if (colorShiftRPerAngle < 0) {
				if (rising) {
					currSunColor.x += -colorShiftRPerAngle;
				}
				else {
					currSunColor.x += colorShiftRPerAngle;
				}
			}
			else if (colorShiftRPerAngle > 0) {
				if (rising) {
					currSunColor.x += colorShiftRPerAngle;
				}
				else {
					currSunColor.x += -colorShiftRPerAngle;
				}
			}
			float colorShiftGPerAngle = (SunColorA.y - SunColorB.y) / 30.0f;
			if (colorShiftGPerAngle < 0) {
				if (rising) {
					currSunColor.y += -colorShiftGPerAngle;
				}
				else {
					currSunColor.y += colorShiftGPerAngle;
				}
			}
			else if (colorShiftGPerAngle > 0) {
				if (rising) {
					currSunColor.y += colorShiftGPerAngle;
				}
				else {
					currSunColor.y += -colorShiftGPerAngle;
				}
			}
			float colorShiftBPerAngle = (SunColorA.z - SunColorB.z) / 30.0f;
			if (colorShiftBPerAngle < 0) {
				if (rising) {
					currSunColor.z += -colorShiftBPerAngle;
				}
				else {
					currSunColor.z += colorShiftBPerAngle;
				}
			}
			else if (colorShiftBPerAngle > 0) {
				if (rising) {
					currSunColor.z += colorShiftBPerAngle;
				}
				else {
					currSunColor.z += -colorShiftBPerAngle;
				}
			}

			SunLightColor = currSunColor;
		} 
		//sun mid
		else if (rotationTracker >= 30 && rotationTracker < 60 || rotationTracker >= 300 && rotationTracker < 330) {
			bool rising = true;
			if (rotationTracker >= 300 && rotationTracker < 330) {
				rising = false;
			}
			float colorShiftRPerAngle = (SunColorB.x - SunColorC.x) / 30.0f;
			if (colorShiftRPerAngle < 0) {
				if (rising) {
					currSunColor.x += -colorShiftRPerAngle;
				}
				else {
					currSunColor.x += colorShiftRPerAngle;
				}
			}
			else if (colorShiftRPerAngle > 0) {
				if (rising) {
					currSunColor.x += colorShiftRPerAngle;
				}
				else {
					currSunColor.x += -colorShiftRPerAngle;
				}
			}
			float colorShiftGPerAngle = (SunColorB.y - SunColorC.y) / 30.0f;
			if (colorShiftGPerAngle < 0) {
				if (rising) {
					currSunColor.y += -colorShiftGPerAngle;
				}
				else {
					currSunColor.y += colorShiftGPerAngle;
				}
			}
			else if (colorShiftGPerAngle > 0) {
				if (rising) {
					currSunColor.y += colorShiftGPerAngle;
				}
				else {
					currSunColor.y += -colorShiftGPerAngle;
				}
			}
			float colorShiftBPerAngle = (SunColorB.z - SunColorC.z) / 30.0f;
			if (colorShiftBPerAngle < 0) {
				if (rising) {
					currSunColor.z += -colorShiftBPerAngle;
				}
				else {
					currSunColor.z += colorShiftBPerAngle;
				}
			}
			else if (colorShiftBPerAngle > 0) {
				if (rising) {
					currSunColor.z += colorShiftBPerAngle;
				}
				else {
					currSunColor.z += -colorShiftBPerAngle;
				}
			}

			SunLightColor = currSunColor;
		}
		//sunset
		else if (rotationTracker >= 60 && rotationTracker < 90 || rotationTracker >= 270 && rotationTracker < 300) {
			bool rising = true;
			if (rotationTracker >= 270 && rotationTracker < 300) {
				rising = false;
			}
			float colorShiftRPerAngle = (SunColorC.x - SunColorA.x) / 30.0f;
			if (colorShiftRPerAngle < 0) {
				if (rising) {
					currSunColor.x += -colorShiftRPerAngle;
				}
				else {
					currSunColor.x += colorShiftRPerAngle;
				}
			}
			else if (colorShiftRPerAngle > 0) {
				if (rising) {
					currSunColor.x += colorShiftRPerAngle;
				}
				else {
					currSunColor.x += -colorShiftRPerAngle;
				}
			}
			float colorShiftGPerAngle = (SunColorC.y - SunColorA.y) / 30.0f;
			if (colorShiftGPerAngle < 0) {
				if (rising) {
					currSunColor.y += -colorShiftGPerAngle;
				}
				else {
					currSunColor.y += colorShiftGPerAngle;
				}
			}
			else if (colorShiftGPerAngle > 0) {
				if (rising) {
					currSunColor.y += colorShiftGPerAngle;
				}
				else {
					currSunColor.y += -colorShiftGPerAngle;
				}
			}
			float colorShiftBPerAngle = (SunColorC.z - SunColorA.z) / 30.0f;
			if (colorShiftBPerAngle < 0) {
				if (rising) {
					currSunColor.z += -colorShiftBPerAngle;
				}
				else {
					currSunColor.z += colorShiftBPerAngle;
				}
			}
			else if (colorShiftBPerAngle > 0) {
				if (rising) {
					currSunColor.z += colorShiftBPerAngle;
				}
				else {
					currSunColor.z += -colorShiftBPerAngle;
				}
			}

			SunLightColor = currSunColor;
		}
		//moon rise 
		else if (rotationTracker >=90 && rotationTracker < 120 || rotationTracker >= 240 && rotationTracker < 270) {
			bool rising = true;
			if (rotationTracker >= 240 && rotationTracker < 270) {
				rising = false;
			}
			float colorShiftRPerAngle = (MoonColorA.x - MoonColorB.x) / 30.0f;
			if (colorShiftRPerAngle < 0) {
				if (rising) {
					currMoonColor.x += -colorShiftRPerAngle;
				}
				else {
					currMoonColor.x += colorShiftRPerAngle;
				}
			}
			else if (colorShiftRPerAngle > 0) {
				if (rising) {
					currMoonColor.x += colorShiftRPerAngle;
				}
				else {
					currMoonColor.x += -colorShiftRPerAngle;
				}
			}
			float colorShiftGPerAngle = (MoonColorA.y - MoonColorB.y) / 30.0f;
			if (colorShiftGPerAngle < 0) {
				if (rising) {
					currMoonColor.y += -colorShiftGPerAngle;
				}
				else {
					currMoonColor.y += colorShiftGPerAngle;
				}
			}
			else if (colorShiftGPerAngle > 0) {
				if (rising) {
					currMoonColor.y += colorShiftGPerAngle;
				}
				else {
					currMoonColor.y += -colorShiftGPerAngle;
				}
			}
			float colorShiftBPerAngle = (MoonColorA.z - MoonColorB.z) / 30.0f;
			if (colorShiftBPerAngle < 0) {
				if (rising) {
					currMoonColor.z += -colorShiftBPerAngle;
				}
				else {
					currMoonColor.z += colorShiftBPerAngle;
				}
			}
			else if (colorShiftBPerAngle > 0) {
				if (rising) {
					currMoonColor.z += colorShiftBPerAngle;
				}
				else {
					currMoonColor.z += -colorShiftBPerAngle;
				}
			}

			MoonLightColor = currMoonColor;
		}
		//moon mid 
		else if (rotationTracker >= 120 && rotationTracker < 150 || rotationTracker >= 210 && rotationTracker < 240) {
			bool rising = true;
			if (rotationTracker >= 210 && rotationTracker < 240) {
				rising = false;
			}
			float colorShiftRPerAngle = (MoonColorB.x - MoonColorC.x) / 30.0f;
			if (colorShiftRPerAngle < 0) {
				if (rising) {
					currMoonColor.x += -colorShiftRPerAngle;
				}
				else {
					currMoonColor.x += colorShiftRPerAngle;
				}
			}
			else if (colorShiftRPerAngle > 0) {
				if (rising) {
					currMoonColor.x += colorShiftRPerAngle;
				}
				else {
					currMoonColor.x += -colorShiftRPerAngle;
				}
			}
			float colorShiftGPerAngle = (MoonColorB.y - MoonColorC.y) / 30.0f;
			if (colorShiftGPerAngle < 0) {
				if (rising) {
					currMoonColor.y += -colorShiftGPerAngle;
				}
				else {
					currMoonColor.y += colorShiftGPerAngle;
				}
			}
			else if (colorShiftGPerAngle > 0) {
				if (rising) {
					currMoonColor.y += colorShiftGPerAngle;
				}
				else {
					currMoonColor.y += -colorShiftGPerAngle;
				}
			}
			float colorShiftBPerAngle = (MoonColorB.z - MoonColorC.z) / 30.0f;
			if (colorShiftBPerAngle < 0) {
				if (rising) {
					currMoonColor.z += -colorShiftBPerAngle;
				}
				else {
					currMoonColor.z += colorShiftBPerAngle;
				}
			}
			else if (colorShiftBPerAngle > 0) {
				if (rising) {
					currMoonColor.z += colorShiftBPerAngle;
				}
				else {
					currMoonColor.z += -colorShiftBPerAngle;
				}
			}

			MoonLightColor = currMoonColor;
		}
		//moon high
		else {
			bool rising = true;
			if (rotationTracker >= 180 && rotationTracker < 210) {
				rising = false;
			}
			float colorShiftRPerAngle = (MoonColorC.x - MoonColorA.x) / 30.0f;
			if (colorShiftRPerAngle < 0) {
				if (rising) {
					currMoonColor.x += -colorShiftRPerAngle;
				}
				else {
					currMoonColor.x += colorShiftRPerAngle;
				}
			}
			else if (colorShiftRPerAngle > 0) {
				if (rising) {
					currMoonColor.x += colorShiftRPerAngle;
				}
				else {
					currMoonColor.x += -colorShiftRPerAngle;
				}
			}
			float colorShiftGPerAngle = (MoonColorC.y - MoonColorA.y) / 30.0f;
			if (colorShiftGPerAngle < 0) {
				if (rising) {
					currMoonColor.y += -colorShiftGPerAngle;
				}
				else {
					currMoonColor.y += colorShiftGPerAngle;
				}
			}
			else if (colorShiftGPerAngle > 0) {
				if (rising) {
					currMoonColor.y += colorShiftGPerAngle;
				}
				else {
					currMoonColor.y += -colorShiftGPerAngle;
				}
			}
			float colorShiftBPerAngle = (MoonColorC.z - MoonColorA.z) / 30.0f;
			if (colorShiftBPerAngle < 0) {
				if (rising) {
					currMoonColor.z += -colorShiftBPerAngle;
				}
				else {
					currMoonColor.z += colorShiftBPerAngle;
				}
			}
			else if (colorShiftBPerAngle > 0) {
				if (rising) {
					currMoonColor.z += colorShiftBPerAngle;
				}
				else {
					currMoonColor.z += -colorShiftBPerAngle;
				}
			}

			MoonLightColor = currMoonColor;
		}
	}
#pragma endregion colorlogic - Philippe

	// Update models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		(*it)->Update(dt);
	}
}

void World::Draw()
{
	Renderer::BeginFrame();
	
	// Set Shader... In a more sofisticated application, each model could use a different shader
	// In our case, all the models use a common shader
	glUseProgram(Renderer::GetShaderProgramID());

	GLuint ViewMatrixID = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewTransform");
	GLuint ProjMatrixID = glGetUniformLocation(Renderer::GetShaderProgramID(), "ProjectonTransform");

	// Get a handle for Light Attributes uniform for World Lighting
	GLuint LightPositionID = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldLightPositions");
	GLuint LightColorID = glGetUniformLocation(Renderer::GetShaderProgramID(), "lightColor");
	GLuint LightAttenuationID = glGetUniformLocation(Renderer::GetShaderProgramID(), "lightAttenuation");
	GLuint NumLightsID = glGetUniformLocation(Renderer::GetShaderProgramID(), "NUM_LIGHTS");

	/* Light positions and colors indeces - Phil & Francis */
	GLfloat* lightPositions = new GLfloat[50];

	for (int i = 0; i < 4; i++) {
		lightPositions[i] = GetVertex(sunlightPosition, i);
	}

	for (int j = 4; j < 8; j++) {
		lightPositions[j] = GetVertex(moonlightPosition, j - 4);
	}

	for (int k = 8; k < 12; k++) {
		lightPositions[k] = GetVertex(bulblightPosition, k - 8);
	}


	GLfloat* lightColors = new GLfloat[50];

	for (int i = 0; i < 3; i++) {
		lightColors[i] = GetVertex(vec4(SunLightColor, 0.0), i);
	}

	for (int j = 3; j < 6; j++) {
		lightColors[j] = GetVertex(vec4(MoonLightColor, 0.0), j - 3);
	}

	for (int k = 6; k < 9; k++) {
		lightColors[k] = GetVertex(vec4(BulbLightColor, 0.0), k - 6);
	}

	glFogi(GL_FOG_COORD_SRC, GL_FRAGMENT_DEPTH);

	/* House and window boundaries - Francis */
	// Window boundaries ID
	GLuint WindowXRangeID = glGetUniformLocation(Renderer::GetShaderProgramID(), "windowRangeX");
	GLuint WindowYRangeID = glGetUniformLocation(Renderer::GetShaderProgramID(), "windowRangeY");
	GLuint WindowZRangeID = glGetUniformLocation(Renderer::GetShaderProgramID(), "windowRangeZ");
	GLuint WindowPlaneID = glGetUniformLocation(Renderer::GetShaderProgramID(), "L");

	// House boundaries ID
	GLuint HouseXRangeID = glGetUniformLocation(Renderer::GetShaderProgramID(), "houseRangeX");
	GLuint HouseYRangeID = glGetUniformLocation(Renderer::GetShaderProgramID(), "houseRangeY");
	GLuint HouseZRangeID = glGetUniformLocation(Renderer::GetShaderProgramID(), "houseRangeZ");

	std::vector<glm::vec2> windowBoundaries = wfm->GetWindowRange(); // Window boundaries
	glm::vec4 windowNormal = wfm->GetPlaneEquation(); // Window plane equation
	std::vector<glm::vec2> houseBoundaries = hm->GetHouseBoundaries(); // House boundaries

	// Pass boundaries to the fragment shader
	glUniform2f(HouseXRangeID, houseBoundaries[0].x, houseBoundaries[0].y);
	glUniform2f(HouseYRangeID, houseBoundaries[1].x, houseBoundaries[1].y);
	glUniform2f(HouseZRangeID, houseBoundaries[2].x, houseBoundaries[2].y);

	glUniform2f(WindowXRangeID, windowBoundaries[0].x, windowBoundaries[0].y);
	glUniform2f(WindowYRangeID, windowBoundaries[1].x, windowBoundaries[1].y);
	glUniform2f(WindowZRangeID, windowBoundaries[2].x, windowBoundaries[2].y);

	// Pass window vector normal (plane) to the fragment shader
	glUniform4f(WindowPlaneID, windowNormal.x, windowNormal.y, windowNormal.z, windowNormal.w);
	/* \House and window boundaries */

	// Draw models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		// Send the Projection and View constants to the shader
		mat4 ProjMatrix = mCamera[mCurrentCamera]->GetProjectionMatrix();
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &ProjMatrix[0][0]);
		
		mat4 ViewMatrix = mCamera[mCurrentCamera]->GetViewMatrix();
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		// Send the World Lighting constants to the shader
		glUniform1i(NumLightsID, 3);
		glUniform4fv(LightPositionID, 3, lightPositions);
		glUniform3fv(LightColorID, 3, lightColors);
		glUniform3f(LightAttenuationID, 0.0, 0.0, 0.05);

		// Draw model
		(*it)->Draw();
	}

	Renderer::EndFrame();
}

/**
 * GetVertex
 *
 * Returns value of vertex position
 *
 * @param vertexPosition - vertex
 * @param index - vertex index
 * @return vertex value 
 */
float World::GetVertex(glm::vec4 vertexPosition, int index)
{
	switch (index) {
		case 1: return vertexPosition.y; break;
		case 2: return vertexPosition.z; break;
		case 3: return vertexPosition.w; break;
		case 0:
		default: return vertexPosition.x; break;
	}
}

void World::LoadScene(const char * scene_path)
{
	// Using case-insensitive strings and streams for easier parsing
	ci_ifstream input;
	input.open(scene_path, ios::in);

	// Invalid file
	if(input.fail() )
	{	 
		fprintf(stderr, "Error loading file: %s\n", scene_path);
		getchar();
		exit(-1);
	}

	ci_string item;
	while( std::getline( input, item, '[' ) )   
	{
        ci_istringstream iss( item );

		ci_string result;
		if( std::getline( iss, result, ']') )
		{
			if( result == "cube" )
			{
				// Box attributes
				CubeModel* cube = new CubeModel();
				cube->Load(iss);
				mModel.push_back(cube);
			}
			else  if (result == "RubiksCube")
			{
				RubiksCubeModel* rcm = new RubiksCubeModel();
				rcm->Load(iss);
				mModel.push_back(rcm);
			}
			else  if (result == "Table")
			{
				TableModel* tm = new TableModel();
				tm->Load(iss);
				mModel.push_back(tm);
			}
			else  if (result == "Sun")
			{
				Sphere* sun = new Sun();
				sun->Load(iss);
				mModel.push_back(sun);
				sunIndex = mModel.size() - 1;
				//mLightSources.push_back(new LightSource(glm::vec3(1.0, 1.0, 1.0), 0.0f, 0.0f, 0.5f, vec4(sun->GetPosition(), 1)));
				//sunlightIndex = mLightSources.size() - 1;
			}
			else  if (result == "Moon")
			{
				Sphere* moon = new Moon();
				moon->Load(iss);
				mModel.push_back(moon);
				moonIndex = mModel.size() - 1;
				//mLightSources.push_back(new LightSource(glm::vec3(1.0, 1.0, 1.0), 0.0f, 0.0f, 0.5f, vec4(moon->GetPosition(), 1)));
				//moonlightIndex = mLightSources.size() - 1;
			}
			else if (result == "HouseDoor")
			{
				this->dm = new DoorModel();
				dm->Load(iss);
				mModel.push_back(this->dm);
			}
			else if (result == "House")
			{
				hm = new HouseModel();
				hm->Load(iss);
				mModel.push_back(hm);
			}
			else if (result == "LightSwitch")
			{
				LightSwitchModel* ls = new LightSwitchModel(BulbLightColor);
				ls->Load(iss);
				mModel.push_back(ls);
			}
			else if (result == "Sofa")
			{
				SofaModel* sm = new SofaModel();
				sm->Load(iss);
				mModel.push_back(sm);
			}
			else  if (result == "Grass")
			{
				this->rcm = new Grass();
				rcm->Load(iss);
				mModel.push_back(this->rcm);
			}
			else if (result == "LightBulb")
			{
				LightBulbModel* lbm = new LightBulbModel();
				lbm->Load(iss);
				mModel.push_back(lbm);
			}
			else if (result == "Skybox1")
			{
				Skybox * sky1 = new Skybox(1);
				sky1->Load(iss);
				mModel.push_back(sky1);
				skyboxes.push_back(sky1);
			}
			else if (result == "Skybox2")
			{
				Skybox * sky2 = new Skybox(2);
				sky2->Load(iss);
				mModel.push_back(sky2);
				skyboxes.push_back(sky2);
			}
			else if (result == "Skybox3")
			{
				Skybox * sky3 = new Skybox(3);
				sky3->Load(iss);
				mModel.push_back(sky3);
				skyboxes.push_back(sky3);
			}
			else if (result == "Skybox4")
			{
				Skybox * sky4 = new Skybox(4);
				sky4->Load(iss);
				mModel.push_back(sky4);
				skyboxes.push_back(sky4);
			}
			else if (result == "WindowFrame")
			{
				wfm = new WindowFrameModel();
				wfm->Load(iss);
				mModel.push_back(wfm);
			}
			else if (result == "Cloud")
			{
				Cloud* cloud = new Cloud();
				cloud->Load(iss);
				mModel.push_back(cloud);
			}
			else if ( result.empty() == false && result[0] == '#')
			{
				// this is a comment line
			}
			else
			{
				fprintf(stderr, "Error loading scene file... !");
				getchar();
				exit(-1);
			}
	    }
	}
	currSkybox = skyboxes.size() - 1;
	input.close();
}

Camera* World::GetCurrentCamera()
{
	return mCamera[mCurrentCamera];
}

Grass* World::GetGrassModel()
{
	return this->rcm;
}

DoorModel* World::GetDoorModel()
{
	return this->dm;
}