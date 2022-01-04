#pragma once
#include <engine.h>

class quad
{
public:

	quad(glm::vec2 half_extents);
	~quad();

	glm::vec2 half_extents() const { return m_half_extents; }
	engine::ref<engine::mesh> mesh() const { return m_mesh; }
	static engine::ref<quad> create(glm::vec2 half_extents);

private:

	glm::vec2 m_half_extents;
	engine::ref<engine::mesh> m_mesh;
};

