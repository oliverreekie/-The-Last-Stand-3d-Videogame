#include "pch.h"
#include "timerBackground.h"
#include "quad.h"

timerBackground::timerBackground(const std::string& path, float width, float height)
{
	//Creates the quad for the timerBackground screen
	m_texture = engine::texture_2d::create(path, true);
	m_quad = quad::create(glm::vec2(width, height));
	//Stores the active state of the timerBackground screen
	s_active = false;
}

timerBackground::~timerBackground()
{}

void timerBackground::on_update()
{
	if (!s_active)
		return;

}

void timerBackground::on_render(engine::ref<engine::shader> shader)
{
	if (!s_active)
		return;
	//Renders the timerBackground screen
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(1.3f, 0.42f, -1.f));
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", true);
	m_texture->bind();
	engine::renderer::submit(shader, m_quad->mesh(), transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", false);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", 1.0f);
}
//Activates the controls screen
void timerBackground::activate()
{
	s_active = true;
}
//Deactivates the controls screen
void timerBackground::deactivate() {
	s_active = false;
}

engine::ref<timerBackground> timerBackground::create(const std::string& path, float width, float height)
{
	return std::make_shared<timerBackground>(path, width, height);
}
