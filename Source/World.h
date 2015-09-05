// World.h
// Contributors:
// @author Philippe Thibault
//			- added alpha algorithm for skyboxes to seamlessly switch between each other
//			- multiple light sources for the sun and moon
//			- rotate sun and moon as well as their light sources around the origin
//          - interpolate colors as the sun and moon move around the scene

//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.
//

#pragma once

#include <vector>

#include <GLM/glm.hpp>
#include "Skybox.h"
#include "Grass.h"
#include "DoorModel.h"
#include "HouseModel.h"
#include "WindowFrameModel.h"

class Camera;
class Model;

class World
{
public:
	World();
	~World();
	
	void Update(float dt);
	void Draw();
	void GetNormal();
	void LoadScene(const char * scene_path);
	Camera* GetCurrentCamera();
	Grass* GetGrassModel();
	DoorModel* GetDoorModel();
	WindowFrameModel* wfm;
	HouseModel* hm;
	float GetVertex(glm::vec4, int);
private:
	std::vector<Model*> mModel;
	Grass* rcm;
	DoorModel* dm;
	std::vector<Camera*> mCamera;
	std::vector<Skybox*> skyboxes;
	unsigned int mCurrentCamera;

	// Light Coefficients
	//-----SUN------//
	glm::vec3 SunColorA = glm::vec3((242.0f / 255.0f), (66.0f / 255.0f), (7.0f / 255.0f));
	glm::vec3 SunColorB = glm::vec3((219.0f / 255.0f), (122.0f / 255.0f), (44.0f / 255.0f));
	glm::vec3 SunColorC = glm::vec3((231.0f / 255.0f), (242.0f / 255.0f), (7.0f / 255.0f));

	glm::vec3 currSunColor = SunColorA;

	glm::vec3 SunLightColor;
	const glm::vec3 SunAttentuation = glm::vec3(0.0f, 0.0f, 0.05f); //kc kl kq
	//-----MOON------//
	glm::vec3 MoonColorA = glm::vec3((7.0f / 255.0f), (242.0f / 255.0f), (231.0f / 255.0f));
	glm::vec3 MoonColorB = glm::vec3((146.0f / 255.0f), (241.0f / 255.0f), (247.0f / 255.0f));
	glm::vec3 MoonColorC = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec3 currMoonColor = MoonColorA;

	glm::vec3 MoonLightColor;
	const glm::vec3 MoonAttentuation = glm::vec3(0.0f, 0.0f, 0.05f); //kc kl kq

	glm::vec3 BulbLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 BulbAttenuation = glm::vec3(0.0f, 0.0f, 2.0f); //kc kl kq

	glm::vec4 sunlightPosition; //= glm::vec4(0.0f, 5.0f, 0.0f, 1.0f); // If w = 1.0f, we have a point light
	glm::vec4 moonlightPosition; //= glm::vec4(0.0f, 5.0f, 0.0f, 1.0f); // If w = 1.0f, we have a point light
	glm::vec4 bulblightPosition = glm::vec4(-2.0f, 1.2f, -1.78f, 1.0f);

	bool mouseClickEnable = true;
};
