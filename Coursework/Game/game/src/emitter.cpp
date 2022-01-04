#include "pch.h"
#include "emitter.h"

emitter::emitter()
{
	s_active = false;
}


emitter::~emitter()
{}

void emitter::load_texture(char* filename)
{
	// Load the texture
	m_texture = engine::texture_2d::create((char*)filename, false); // Loads an image.  Make sure the image is in a 32 bit format (tga) with transparency

	//create a quad
	// Determine the coordinates of the quad	   	 
	std::vector<engine::mesh::vertex> quad_vertices
	{
		//			position					normal					  tex coord       
		{ glm::vec3(-1.f, -1.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),		{ 0.f, 0.f } },
		{ glm::vec3(-1.f, 1.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),		{ 0.f, 1.f } },
		{ glm::vec3(1.f, 1.f, 0.f),			glm::vec3(0.f, 0.f, -1.f),		{ 1.f, 1.f } },
		{ glm::vec3(1.f, -1.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),		{ 1.f, 0.f } },
	};

	const std::vector<uint32_t> quad_indices
	{
		 0,  1,  2,
		 0,  2,  3,
		 0,  2,  1,
		 0,  3,  2,
	};

	m_quad = engine::mesh::create(quad_vertices, quad_indices);
}

void emitter::initialise(glm::vec3 startPos, glm::vec3 startDir)
{
	//Sets the start position and direction
	m_start = startPos;
	m_direction = startDir;
	glm::normalize(m_direction);
	m_direction = m_direction * 2.0f;

	//Sets the width and height of fire
	m_width = 0.8f;
	m_height = 1.3f;

	s_active = true;

	//Add 18 particles
	for (int i = 0; i < 18; i++) {
		add_particle();
	}
}

void emitter::clear()
{
	//Remove all the particles
	m_particles.clear();
	s_active = false;
}

void emitter::on_render(const engine::perspective_camera& camera, engine::ref<engine::shader> shader)
{
	if (!s_active)
		return;

	m_texture->bind();

	// Render each particle
	for (unsigned int i = 0; i < m_particles.size(); i++)
	{
			std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("lighting_on", false);
			std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("colouring_on", true);
			std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", true);
			engine::renderer_api::disable_depth_mask();
			m_texture->bind();
			glm::mat4 transform = m_particles[i].on_render(camera.front_vector(), camera.up_vector(), shader);
			engine::renderer::submit(shader, m_quad, transform);
			engine::renderer_api::enable_depth_mask();
			std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", false);
			std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("colouring_on", false);
			std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("lighting_on", true);
	}
}

void emitter::on_update(const engine::timestep& timestep)
{
	//Update each of the particles
	for (unsigned int i = 0; i < m_particles.size(); i++) {
		m_particles[i].on_update(timestep);
	}
}

// Add another particle 
void emitter::add_particle()
{
	particle particle;
	glm::vec3 position = m_start + (float)m_particles.size() * m_width * m_direction / (float)26;
	particle.initialise(position, m_width, m_height, glm::vec3(200.f, 1.f, 1.f), glm::vec3(1.f), 15);
	m_particles.push_back(particle);
}
