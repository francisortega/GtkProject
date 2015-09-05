//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.
//

#include "Camera.h"
#include "EventManager.h"

#include <GLM/gtx/transform.hpp>

Camera::Camera()
{
	aspect = EventManager::GetAspect();
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewProjectionMatrix() const
{
	// @TODO 1 - Calculate View Projection Matrix
	//glm::mat4 viewProjection(1.0f); // identity, you need to change this

	glm::mat4 viewProjection = GetProjectionMatrix() * GetViewMatrix();

	return viewProjection;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(45.0f, aspect, 0.1f, 400.0f);
}

void Camera::SetAspect(float aspect)
{
	this->aspect = aspect;
}