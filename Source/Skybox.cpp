// Skybox.cpp
// Purpose: A model representing the skybox around the scene

// @author Philippe Thibault
#include "Skybox.h"

#include "Renderer.h"
#include "Texture.h"


// Include GLEW - OpenGL Extension Wrangler
#define GLEW_STATIC
#include <GL/glew.h>

using namespace glm;


Skybox::Skybox(int type, Model* parent) : Model(parent)
{
	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> uvs;
	std::vector<glm::vec3> normals;

	this->type = type;

	if (type == 1) {
		TextureOne = Texture::loadDDS("../Source/Models/Skybox/day.dds");
		TextureIdOne = glGetUniformLocation(Renderer::GetShaderProgramID(), "mTexture");

		bool res = Renderer::LoadOBJ("../Source/Models/Skybox/skybox.obj", vertices, uvs, normals);

	}
	else if (type == 2) {
		TextureTwo = Texture::loadDDS("../Source/Models/Skybox/afternoon.dds");
		TextureIdTwo = glGetUniformLocation(Renderer::GetShaderProgramID(), "mTexture");

		bool res = Renderer::LoadOBJ("../Source/Models/Skybox/skybox.obj", vertices, uvs, normals);
	}
	else if (type == 3) {
		TextureThree = Texture::loadDDS("../Source/Models/Skybox/sunset.dds");
		TextureIdThree = glGetUniformLocation(Renderer::GetShaderProgramID(), "mTexture");

		bool res = Renderer::LoadOBJ("../Source/Models/Skybox/skybox.obj", vertices, uvs, normals);
	}
	else {
		TextureFour = Texture::loadDDS("../Source/Models/Skybox/night.dds");
		TextureIdFour = glGetUniformLocation(Renderer::GetShaderProgramID(), "mTexture");

		bool res = Renderer::LoadOBJ("../Source/Models/Skybox/skybox.obj", vertices, uvs, normals);
	}


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


Skybox::~Skybox()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteBuffers(1, &mNormalBufferID);
	glDeleteBuffers(1, &mUVBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}

void Skybox::Update(float dt)
{

}

void Skybox::Draw()
{
	GLuint WorldMatrixID = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &GetWorldMatrix()[0][0]);

	GLuint MaterialID = glGetUniformLocation(Renderer::GetShaderProgramID(), "materialCoefficients");
	glUniform4f(MaterialID, ka, kd, ks, n);

	GLuint ModelTransparencyID = glGetUniformLocation(Renderer::GetShaderProgramID(), "modelTransparency");
	glUniform1f(ModelTransparencyID, tr);

	GLuint NoShadingID = glGetUniformLocation(Renderer::GetShaderProgramID(), "noShading");
	glUniform1f(NoShadingID, noShading);

	glActiveTexture(GL_TEXTURE0);
	if (type == 1) {
		glBindTexture(GL_TEXTURE_2D, TextureOne);
		// Set our texture sampler to user Texture Unit 0
		glUniform1i(TextureIdOne, 0);
	}
	else if (type == 2){
		glBindTexture(GL_TEXTURE_2D, TextureTwo);
		// Set our texture sampler to user Texture Unit 0
		glUniform1i(TextureIdTwo, 0);
	}
	else if (type == 3){
		glBindTexture(GL_TEXTURE_2D, TextureThree);
		// Set our texture sampler to user Texture Unit 0
		glUniform1i(TextureIdThree, 0);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, TextureFour);
		// Set our texture sampler to user Texture Unit 0
		glUniform1i(TextureIdFour, 0);
	}

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

bool Skybox::ParseLine(const std::vector<ci_string> &token)
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

void Skybox::SetTransparency(float trans) {
	tr = trans;
}
