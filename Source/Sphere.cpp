// Purpose: Added some texture mapping to the spheres depending on type

// @author Philippe Thibault

//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 22/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.
//

#include "sphere.h"
#include "Renderer.h"
#include <glm/glm.hpp>
#include "SphereType.h"
#include "Texture.h"

using namespace glm;

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW - OpenGL Extension Wrangler
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW - OpenGL Framework (Multi Platform framework to Setup a window and OpenGL Context
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM - OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Sphere::Sphere(int sphereType, vec3 size, Model* parentModel) : Model(parentModel)
{
	if (sphereType == SphereType::SUN) {
		// Load the texture 
		Texture = Texture::loadDDS("../Source/Models/Sun/sun_texture.dds");

		// Get a handle for our texture uniform
		TextureID = glGetUniformLocation(Renderer::GetShaderProgramID(), "mTexture");
	}
	else if (sphereType == SphereType::MOON) {
		// Load the texture 
		// Load the texture 
		Texture = Texture::loadDDS("../Source/Models/Moon/moon_texture.dds");

		// Get a handle for our texture uniform
		TextureID = glGetUniformLocation(Renderer::GetShaderProgramID(), "mTexture");
	}

	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> uvs;
	std::vector<glm::vec3> normals;
	bool res = Renderer::LoadOBJ("../Source/Models/Sphere/sun.obj", vertices, uvs, normals);

	numVertices = vertices.size();

	// Create a vertex array
	glGenVertexArrays(1, &mVertexArrayID);

	// Load it into a VBO
	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &mNormalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mNormalBufferID);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &mUVBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec3), &uvs[0], GL_STATIC_DRAW);
}

void Sphere::Update(float dt)
{

}

void Sphere::Draw()
{
	glBindVertexArray(mVertexArrayID);

	GLuint WorldMatrixID = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &GetWorldMatrix()[0][0]);

	GLuint MaterialID = glGetUniformLocation(Renderer::GetShaderProgramID(), "materialCoefficients");
	glUniform4f(MaterialID, ka, kd, ks, n);

	GLuint ModelTransparencyID = glGetUniformLocation(Renderer::GetShaderProgramID(), "modelTransparency");
	glUniform1f(ModelTransparencyID, tr);

	GLuint NoShadingID = glGetUniformLocation(Renderer::GetShaderProgramID(), "noShading");
	glUniform1f(NoShadingID, noShading);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our texture sampler to user Texture Unit 0
	glUniform1i(TextureID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : vertex normal
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mNormalBufferID);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0    // Normal is Offseted by vec3 (see class Vertex)
		);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mUVBufferID);
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, numVertices); // 3 indices starting at 0 -> 1 triangle

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

bool Sphere::ParseLine(const std::vector<ci_string> &token)
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

Sphere::~Sphere() {
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteBuffers(1, &mNormalBufferID);
	glDeleteBuffers(1, &mUVBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}
