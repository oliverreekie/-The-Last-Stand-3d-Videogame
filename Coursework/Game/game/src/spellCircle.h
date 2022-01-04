#pragma once
#include <engine.h>

class spellCircle
{
public:
	spellCircle();
	~spellCircle();
	void initialise();
	void activate(glm::vec3 position);
	void on_render(engine::ref<engine::shader> shader);
	void on_update(const engine::timestep& timestep);
	bool is_active() { return s_active; }

	void expand();
	void disappear();

	float getRadius() {
		return m_radius;
	}

private:
	engine::ref<engine::texture_2d> m_texture;
	engine::ref<engine::mesh> m_quad;
	bool s_active;
	//The radius of the circle
	float m_radius;
	float m_start_radius;
	//The time remaining
	float m_life;
	//The position of the circle
	glm::vec3 m_position;
	//Whether the circle is growing or not
	bool growing;

};
