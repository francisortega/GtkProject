// HouseModel.cpp
// Purpose: A model representing the house.
//
// @author Michael Lavoie

#include "HouseModel.h"

#include "Renderer.h"
#include "Texture.h"
#include "BoundingBox.h"


// Include GLEW - OpenGL Extension Wrangler
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/ext.hpp>

using namespace glm;


HouseModel::HouseModel(Model* parent) : Model(parent)
{
	// Load the texture 
	Texture = Texture::loadDDS("../Source/Models/House/house.dds");

	// Get a handle for our texture uniform
	TextureID = glGetUniformLocation(Renderer::GetShaderProgramID(), "mTexture");

	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> uvs;
	std::vector<glm::vec3> normals;
	bool res = Renderer::LoadOBJ("../Source/Models/House/house.obj", vertices, uvs, normals);

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

	houseBoundaries = { glm::vec2(0), glm::vec2(0), glm::vec2(0) };

	BoundingBox::CalculateBoundaries(vertices, houseBoundaries[0], houseBoundaries[1], houseBoundaries[2]);
	houseBoundaries[1].y = 2.5f;
}


HouseModel::~HouseModel()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteBuffers(1, &mNormalBufferID);
	glDeleteBuffers(1, &mUVBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}

void HouseModel::Update(float dt)
{

}

std::vector<glm::vec2> HouseModel::GetHouseBoundaries()
{
	return houseBoundaries;
}

void HouseModel::Draw()
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

bool HouseModel::ParseLine(const std::vector<ci_string> &token)
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
