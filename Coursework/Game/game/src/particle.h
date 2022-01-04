#pragma once
#include <engine.h>

class particle
{
public:
	particle();
	~particle();
	bool initialise(glm::vec3 p, float width, float height, glm::vec3 start_colour, glm::vec3 end_colour, float transition_speed);
	glm::mat4 on_render(glm::vec3 lookAt, glm::vec3 upVector, engine::ref<engine::shader> shader);
	void on_update(const engine::timestep& timestep);

private:
	float m_life;
	glm::vec3 m_position;
	glm::vec3 m_velocity;

	//Stores the width and height of the flame
	float m_width;
	float m_height;

	glm::vec3 m_start_colour;
	glm::vec3 m_end_colour;
	float m_transition_speed;

	//Stores whether the height will increase or decrease
	bool increase = true;
};
