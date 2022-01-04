#include "pch.h"
#include "spellCircle.h"


spellCircle::spellCircle()
{
	s_active = false;
	m_radius = 2.5f;
	m_start_radius = m_radius;
	growing = false;
}

spellCircle::~spellCircle()
{}

void spellCircle::on_update(const engine::timestep& timestep)
{
	if (s_active == false)
		return;

	//Remove if life is less than 0
	if (m_life < 0) {
		s_active = false;
		return;
	}
	//If growing, increase the radius of the spell circle to cover the ground
	if (growing == true) {
		float f = pow(m_life, 0.25f);
		m_radius += 11.5f * f * (float)timestep;
	}
}

void spellCircle::initialise()
{
	// Load the texture
	m_texture = engine::texture_2d::create("assets/textures/summoningCircle/spellCircle.png", false);

	glm::vec3 x_axis = glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 z_axis = glm::vec3(0.f, 0.f, 1.f);

	// Assume oriented in xz plane
	glm::vec3 normal = glm::vec3(0.f, 1.f, 0.f);

	// Determine the coordinates of the quad	   	 
	std::vector<engine::mesh::vertex> quad_vertices
	{
		//							 position												normal		 tex coord       
		{ glm::vec3(-m_start_radius * x_axis - m_start_radius * z_axis),		normal,		{ 0.f, 0.f } },
		{ glm::vec3(-m_start_radius * x_axis + m_start_radius * z_axis),		normal,		{ 0.f, 1.f } },
		{ glm::vec3(m_start_radius * x_axis + m_start_radius * z_axis),		normal,		{ 1.f, 1.f } },
		{ glm::vec3(m_start_radius * x_axis - m_start_radius * z_axis),		normal,		{ 1.f, 0.f } },
	};

	const std::vector<uint32_t> quad_indices
	{
		 0,  1,  2,
		 0,  2,  3,
	};

	m_quad = engine::mesh::create(quad_vertices, quad_indices);
}

void spellCircle::activate(glm::vec3 position)
{
	//Sets the initial values for circle
	m_position = position;
	m_life = 1.0f;
	m_radius = 2.5f;
	s_active = true;

}

void spellCircle::on_render(engine::ref<engine::shader> shader)
{

	if (s_active == false)
		return;

	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("lighting_on", false);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", true);
	m_texture->bind();
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position);
	transform = glm::scale(transform, glm::vec3(m_radius / m_start_radius));
	engine::renderer::submit(shader, m_quad, transform);
	//std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", transparency);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", false);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("lighting_on", true);

}
//Starts the growing of the circle
void spellCircle::expand() {
	growing = true;
}
//Removes the spell circle
void spellCircle::disappear() {
	s_active = false;
}
