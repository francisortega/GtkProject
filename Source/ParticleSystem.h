// ParticleSystem.h
// Purpose: An bject representing a particle system to be used for clouds and other such objects in the scene
//Note : currently incomplete and unused

// @author Philippe Thibault
#pragma once

#include <GLM/glm.hpp>
#include <GLFW/glfw3.h>

class ParticleSystem
{

public:

	ParticleSystem();
	~ParticleSystem();

	static const int NUM_PARTICLES = 200;
	float advance_amount = 0.01f;

	GLuint texture;
	//Particle particles[NUM_PARTICLES];


};
