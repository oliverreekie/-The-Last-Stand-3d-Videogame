#include "pch.h"
#include "particle.h"

particle::particle()
{}

particle::~particle()
{}

void particle::on_update(const engine::timestep& timestep)
{
	//Record if flame has reached min and max to reverse direction
	if (m_height >= 2.35) {
		increase = false;
	}
	if (m_height <= 1.50) {
		increase = true;
	}

	//Increase or decrease flame size between min and max
	if (increase == true) {
		m_height += 0.3f * (float)timestep;
	}
	else {
		m_height -= 0.3f * (float)timestep;
	}

}


bool particle::initialise(glm::vec3 p, float width, float height, glm::vec3 start_colour, glm::vec3 end_colour, float transition_speed)
{
	// Set the initial position and velocity
	m_position = p;

	//Generate a random height for the flame particle
	int rand1 = rand() % (235 - 150);
	float generateX = (float)rand1;
	generateX = (generateX / 100) + 1.50;

	// Set the sprite colour sequence, width, height
	m_width = width;
	m_height = generateX;

	// Set the particle life
	m_life = 2.0f;

	m_start_colour = start_colour;
	m_end_colour = end_colour;
	m_transition_speed = transition_speed;


	return true;
}


glm::mat4 particle::on_render(glm::vec3 look_at, glm::vec3 up_vector, engine::ref<engine::shader> shader)
{

	glm::vec3 orthogonal_vector = glm::cross(glm::normalize(look_at), up_vector);

	float angle = atan2(look_at.x, look_at.z);

	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, m_position);
	transform = glm::rotate(transform, angle, glm::vec3(0.f, 1.f, 0.f));
	transform = glm::scale(transform, glm::vec3(m_width, m_height, 1.f));

	// Allow faster (m_transitionSpeed > 1) or slower transitions (0 < m_transitionSpeed < 1) between the two colours
	float f = pow(m_life, m_transition_speed);

	// Interpolate between the start and end colour
	glm::vec3 colour = (1 - f) * m_end_colour + f * m_start_colour;

	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("in_colour", colour);

	return transform;
}

