#include "pch.h"
#include "onFire.h"
#include "quad.h"

onFire::onFire(const std::string& path, float width, float height)
{
	//Creates the quad for the onFire screen
	m_texture = engine::texture_2d::create(path, true);
	m_quad = quad::create(glm::vec2(width, height));
	//Stores the active state of the onFire screen
	s_active = false;
	//This marks the maximum time for the effect, after which it will delete
	m_max_time = 8.0f;
}

onFire::~onFire()
{}

void onFire::on_update(const engine::timestep& time_step)
{
	//If the effect is no longer active, stop updating
	if (!s_active)
		return;
	//Keeps a running timer with frame rate
	m_timer += (float)time_step;

	//Reverses direction of transparency change if max or min values are reached
	if (m_transparency >= 0.10) {
		increase = false;
	}
	if (m_transparency <= 0.02) {
		increase = true;
	}
	//If increasing then decrease transparency based on frame rate
	if (increase == true) {
		m_transparency += 0.01f * (float)time_step;
	}
	//If decreaseing then increase transparency based on frame rate
	else {
		m_transparency -= 0.01f * (float)time_step;
	}
	//When max time (10.f) is met, stop updating
	if (m_timer > m_max_time)
		s_active = false;
}

void onFire::on_render(engine::ref<engine::shader> shader)
{
	if (!s_active)
		return;
	//Renders the onFire screen
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(0.f, 0.f, -0.6f));
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", m_transparency);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", true);
	m_texture->bind();
	engine::renderer::submit(shader, m_quad->mesh(), transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", false);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", 1.0f);
}
//Activates the controls screen
void onFire::activate()
{
	//Records effect as active
	s_active = true;
	//Sets initial transparency to max (0.8f)
	m_transparency = 0.05f;
	//Start timer from zero
	m_timer = 0.0f;
	//Set Transparency to initially decrease
	increase = false;
}
//Deactivates the effect
void onFire::deactivate() {
	//Records active state as false
	s_active = false;
}

engine::ref<onFire> onFire::create(const std::string& path, float width, float height)
{
	return std::make_shared<onFire>(path, width, height);
}
