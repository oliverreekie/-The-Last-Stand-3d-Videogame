#include "pch.h"
#include "healthBar.h"
#include "quad.h"

healthBar::healthBar(const std::string& path, float width, float height)
{
	//Creates the quad for the healthBar screen
	m_texture = engine::texture_2d::create(path, true);
	m_quad = quad::create(glm::vec2(width, height));
	//Stores the active state of the healthBar screen
	s_active = false;
}

healthBar::~healthBar()
{}

void healthBar::on_update()
{
	if (!s_active)
		return;

}

void healthBar::on_render(engine::ref<engine::shader> shader)
{
	if (!s_active)
		return;
	//Renders the healthBar screen
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(1.1f, 0.70f, -0.3f));
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", true);
	m_texture->bind();
	engine::renderer::submit(shader, m_quad->mesh(), transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", false);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", 1.0f);
}
//Activates the controls screen
void healthBar::activate()
{
	s_active = true;
}
//Deactivates the controls screen
void healthBar::deactivate() {
	s_active = false;
}

engine::ref<healthBar> healthBar::create(const std::string& path, float width, float height)
{
	return std::make_shared<healthBar>(path, width, height);
}
