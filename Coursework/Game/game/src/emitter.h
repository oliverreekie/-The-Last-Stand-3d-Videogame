#pragma once
#include <engine.h>
#include "particle.h"

class emitter
{

public:

	emitter();
	~emitter();
	void initialise(glm::vec3 smoke_trail_start, glm::vec3 smoke_trail_direction);
	void on_update(const engine::timestep& timestep);
	void on_render(const engine::perspective_camera& camera, engine::ref<engine::shader> shader);
	void load_texture(char* filename);
	bool is_active() { return s_active; }
	void clear();

private:

	engine::ref<engine::texture_2d> m_texture;
	engine::ref<engine::mesh> m_quad;

	//Stores the particles
	std::vector<particle> m_particles;

	//Holds the start and direction of the particles
	glm::vec3 m_start;
	glm::vec3 m_direction;

	//Stores the width and heigh of the fire
	float m_width;
	float m_height;
	bool s_active;

	//Method to add a particle
	void add_particle();
};
