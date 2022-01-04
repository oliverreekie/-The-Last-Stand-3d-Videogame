#include "pch.h"
#include "soulHolder.h"
#include "quad.h"

soulHolder::soulHolder(const std::string& path, float width, float height)
{
	//Creates the quad for the soulHolder screen
	m_texture = engine::texture_2d::create(path, true);
	m_quad = quad::create(glm::vec2(width, height));
	//Stores the active state of the soulHolder screen
	s_active = false;
}

soulHolder::~soulHolder()
{}

void soulHolder::on_update()
{
	if (!s_active)
		return;

}

void soulHolder::on_render(engine::ref<engine::shader> shader)
{
	if (!s_active)
		return;
	//Renders the soulHolder screen
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(-1.3f, 0.50f, -0.3f));
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", true);
	m_texture->bind();
	engine::renderer::submit(shader, m_quad->mesh(), transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", false);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", 1.0f);
}
//Activates the controls screen
void soulHolder::activate()
{
	s_active = true;
}
//Deactivates the controls screen
void soulHolder::deactivate() {
	s_active = false;
}

engine::ref<soulHolder> soulHolder::create(const std::string& path, float width, float height)
{
	return std::make_shared<soulHolder>(path, width, height);
}
