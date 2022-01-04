#include "pch.h"
#include "intro.h"
#include "quad.h"

intro::intro(const std::string& path, float width, float height)
{
	//Creates the quad for the intro screen
	m_texture = engine::texture_2d::create(path, true);
	m_quad = quad::create(glm::vec2(width, height));
	//Stores the active state of the intro screen
	s_active = false;
	//Stores the current transparency
	transparency = 0.0f;
}

intro::~intro()
{}

void intro::on_update(const engine::timestep& time_step)
{
	if (!s_active)
		return;

	//Increase the remaining timer taking into account time step
	remaining = remaining + (float)time_step;

	//If the fade has begun
	if (startFade == true) {
		//Decrease transparency over time
		if (transparency >= 0.0f) {
			transparency = 0.8f - 0.4f * remaining;
		}
		//If transparency is less than 0, deactivate the screen
		else {
			deactivate();
			startFade = false;
		}
	}
}

void intro::on_render(engine::ref<engine::shader> shader)
{
	if (!s_active)
		return;
	//Renders the intro screen
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(0.f, 0.f, -0.3f));
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", transparency);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", true);
	m_texture->bind();
	engine::renderer::submit(shader, m_quad->mesh(), transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", false);

	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", 1.0f);
}
//Activates the controls screen
void intro::activate()
{
	s_active = true;
	transparency = 1.0f;
}
//Deactivates the controls screen
void intro::deactivate() {
	s_active = false;
}

//Begins fade method and starts timer from 0
void intro::startTheFade() {
	startFade = true;
	remaining = 0.0f;
}

engine::ref<intro> intro::create(const std::string& path, float width, float height)
{
	return std::make_shared<intro>(path, width, height);
}
