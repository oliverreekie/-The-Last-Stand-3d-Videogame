#include "pch.h"
#include "quest.h"
#include "quad.h"

quest::quest(const std::string& path, float width, float height)
{
	//Creates the quad for the quest screen
	m_texture = engine::texture_2d::create(path, true);
	m_quad = quad::create(glm::vec2(width, height));
	//Stores the active state of the quest screen
	s_active = false;
}

quest::~quest()
{}

void quest::on_update()
{
	if (!s_active)
		return;

}

void quest::on_render(engine::ref<engine::shader> shader)
{
	if (!s_active)
		return;
	//Renders the quest screen
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(0.f, 0.f, -0.3f));
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", true);
	m_texture->bind();
	engine::renderer::submit(shader, m_quad->mesh(), transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", false);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", 1.0f);
}
//Activates the controls screen
void quest::activate()
{
	s_active = true;
}
//Deactivates the controls screen
void quest::deactivate() {
	s_active = false;
}

engine::ref<quest> quest::create(const std::string& path, float width, float height)
{
	return std::make_shared<quest>(path, width, height);
}
