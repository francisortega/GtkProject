// Cloud.cpp
// Purpose: A model representing the particle system for a cloud
//
// @author Philippe Thibault

#include "Cloud.h"
#include "Renderer.h"
#include <glm/glm.hpp>
#include "Texture.h"
#include "ParticleSystem.h"

using namespace glm;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Particle {
	glm::vec3 position, speed;
	unsigned char r, g, b, a;
	float size;
	float lifetime;
};

const int MaxParticles = 200;
int ParticlesCount = 0;
GLuint billboard_vertex_buffer;
GLfloat particle_pos_buff[4 * MaxParticles];
Particle TotalParticles[MaxParticles];

Cloud::Cloud(vec3 size, Model* parentModel) : Model(parentModel)
{
	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
	};

	float delta = 0.01f;

	for (int i = 0; i<MaxParticles; i++){

		Particle& particle = TotalParticles[i]; // shortcut

		if (particle.lifetime > 0.0f) {
			// Decrease life
			particle.lifetime -= delta;
			if (particle.lifetime > 0.0f){
				particle.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)delta * 0.5f;
				particle.position += particle.speed * (float)delta;

				particle_pos_buff[4 * ParticlesCount + 0] = particle.position.x;
				particle_pos_buff[4 * ParticlesCount + 1] = particle.position.y;
				particle_pos_buff[4 * ParticlesCount + 2] = particle.position.z;

				particle_pos_buff[4 * ParticlesCount + 3] = particle.size;

			}
			ParticlesCount++;

		}
	}
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mVertexBufferID), g_vertex_buffer_data, GL_STATIC_DRAW);

}

void Cloud::Update(float dt)
{
	
}

void Cloud::Draw()
{	

	GLuint particles_position_buffer;
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);

	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	GLuint particles_color_buffer;
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);

	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat)* 4, particle_pos_buff);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(
		0, 
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0 
		);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glVertexAttribPointer(
		1,
		4, 
		GL_FLOAT,
		GL_FALSE, 
		0,
		(void*)0 
		);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1); 
	glVertexAttribDivisor(2, 1); 

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);
}

bool Cloud::ParseLine(const std::vector<ci_string> &token)
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

Cloud::~Cloud() {
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteBuffers(1, &mNormalBufferID);
	glDeleteBuffers(1, &mUVBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}
