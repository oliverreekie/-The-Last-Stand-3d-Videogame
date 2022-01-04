#include "pch.h"
#include "died.h"
#include "quad.h"

died::died(const std::string& path, float width, float height)
{
	//Creates the quad for the died screen
	m_texture = engine::texture_2d::create(path, true);
	m_quad = quad::create(glm::vec2(width, height));
	//Stores the active state of the died screen
	s_active = false;

	//Sets initial transparency to zero
	transparency = 0.0f;
}

died::~died()
{}

void died::on_update(const engine::timestep& time_step)
{
	if (!s_active)
		return;

	//Gradually increases transparency over time (regardless of processor speed)
	transparency += 0.3f * (float)time_step;
}

void died::on_render(engine::ref<engine::shader> shader)
{
	if (!s_active)
		return;
	//Renders the died screen
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(0.f, 0.f, -0.3f));
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", transparency);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", true);
	m_texture->bind();
	engine::renderer::submit(shader, m_quad->mesh(), transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", false);

	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", 1.0f);
}
//Activates the death screen
void died::activate()
{
	//Variable to hold if the screen is active
	s_active = true;
}


engine::ref<died> died::create(const std::string& path, float width, float height)
{
	return std::make_shared<died>(path, width, height);
}
