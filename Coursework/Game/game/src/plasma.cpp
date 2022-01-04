#include "pch.h"
#include "plasma.h"

plasma::plasma(glm::vec3 start_colour, glm::vec3 end_colour, float zap_level, float max_time)
{
	m_sphere = engine::sphere::create(15, 30, 1.0f);
	m_material = engine::material::create(0.0f, start_colour, glm::vec3(0.0f), glm::vec3(0.0f), 0.25f);

	m_start_colour = start_colour;
	m_end_colour = end_colour;

	m_zap_level = zap_level;
	m_max_time = max_time;
	s_active = false;
}

plasma::~plasma()
{}

void plasma::on_update(const engine::timestep& time_step)
{
	if (!s_active)
		return;

	//If the plasma has been released
	if (isReleased == true) {

		//Over time reduce the time remaining for the plasma
		float f = m_timer / m_max_time;

		//Change colour over time
		if (f > 1.0f)
			f = 1.0f;
		m_material->set_ambient((1 - f) * m_start_colour + f * m_end_colour);

		m_timer += (float)time_step;
		if (m_timer > m_max_time) {
			s_active = false;
		}
	}
}


void plasma::on_render(engine::ref<engine::shader> shader)
{
	if (!s_active)
		return;

	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("lighting_on", false);

	//// Draw the lightning (zap) effect with the sphere

		// Rotate the lightning a random amount around a random vector
		glm::vec3 random_axis = glm::vec3(random(), random(), random());
		glm::normalize(random_axis);
		float random_angle = random() * engine::PI;

		// Create a noisy line that is mapped onto the sphere using spherical coordinates
		std::vector<engine::mesh::vertex> line_vertices;
		std::vector<uint32_t> line_indices;
		int N = 100;
		for (int i = 0; i < N; i++) {
			float theta = (i / (float)N) * 2 * engine::PI;
			float phi = engine::PI / 2 + random() * m_zap_level / 2.0f;
			float x = m_scale * cos(theta) * sin(phi);
			float y = m_scale * sin(theta) * sin(phi);
			float z = m_scale * cos(phi);
			line_vertices.push_back(engine::mesh::vertex(glm::vec3(x, y, z), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f)));
			line_indices.push_back(i);
			if (i + 1 < N)
				line_indices.push_back(i + 1);
		}
		engine::ref<engine::mesh> line_mesh = engine::mesh::create(line_vertices, line_indices);

		// Set the colour
		engine::material line_mat = engine::material(0.f, m_material->ambient() + glm::vec3(0.f, 1.0f, 0.f), glm::vec3(0.f), glm::vec3(0.f), 0.75f);
		line_mat.submit(shader);

		engine::renderer_api::line_width(2.0f);

		// Draw the line on the sphere K times with different rotations
		float K = 2;
		for (int i = 0; i < K; i++) {
			glm::mat4 line_transform(1.0f);
			line_transform = glm::translate(line_transform, m_position);
			line_transform = glm::rotate(line_transform, random_angle, random_axis);
			line_transform = glm::rotate(line_transform, engine::PI / K, glm::vec3(0.f, 1.f, 0.f));  // Relative positioning

			std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("u_transform", line_transform);

			line_mesh->va()->bind();
			engine::renderer_api::draw_indexed_lines(line_mesh->va());
		}

	m_material->submit(shader);
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, m_position);
	transform = glm::scale(transform, glm::vec3(m_scale));

	engine::renderer::submit(shader, m_sphere->mesh(), transform);

	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("lighting_on", true);

}

void plasma::activate(float radius, glm::vec3 position)
{
	s_active = true;
	m_position = position;
	m_timer = 0.0f;
	m_scale = radius;
}

engine::ref<plasma> plasma::create(glm::vec3 start_colour, glm::vec3 end_colour, float zap_level, float max_time)
{
	return std::make_shared<plasma>(start_colour, end_colour, zap_level, max_time);
}

// Return a random floating point number between 0 and 1
float plasma::random()
{
	return (rand() / (float)RAND_MAX) * 2.0f - 1.0f;
}

//Set plasma position
void plasma::setPosition(glm::vec3 pos) {
	m_position = pos;
}

//Release the plasma
void plasma::release() {
	isReleased = true;
}
