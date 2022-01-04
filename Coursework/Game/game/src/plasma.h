#pragma once
#include <engine.h>

class plasma
{
public:
	plasma(glm::vec3 start_colour, glm::vec3 end_colour, float zap_level, float max_time);
	~plasma();

	void on_update(const engine::timestep& time_step);
	void on_render(engine::ref<engine::shader> shader);
	void activate(float radius, glm::vec3 position);

	//Release plasma
	void release();

	static engine::ref<plasma> create(glm::vec3 start_colour, glm::vec3 end_colour, float zap_level, float max_time);

	//Set the position
	void setPosition(glm::vec3);

	//Get if the plasma is released
	bool getIsReleased() {
		return isReleased;
	}

private:

	float random();

	//Holds the position and state of the plasma
	glm::vec3 m_position;
	bool s_active;
	float m_max_time;
	float m_timer;
	float m_zap_level;

	//The scale of the plasma
	float m_scale;
	
	glm::vec3 m_start_colour;
	glm::vec3 m_end_colour;
	engine::ref<engine::material> m_material;
	engine::ref<engine::sphere> m_sphere;
	bool s_zap;

	//Holds if the plasma has been released
	bool isReleased = false;
};
