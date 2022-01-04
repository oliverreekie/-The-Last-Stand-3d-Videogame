#include "pch.h"
#include "bounding_box_bullet.h"
#include "platform/opengl/gl_shader.h"
 
engine::bounding_box::bounding_box()
{
	m_width = 0;
	m_height = 0;
	m_depth = 0;
}

engine::bounding_box::~bounding_box()
{}

void engine::bounding_box::get(glm::vec3 & position, float &width, float &height, float &depth)
{
	position = m_position;
	width = m_width;
	height = m_height;
	depth = m_depth;
}

void engine::bounding_box::set_box(float width, float height, float depth, glm::vec3 position)
{
	m_width = width;
	m_height = height;
	m_depth = depth;

	m_position = position;

	std::vector<engine::mesh::vertex> grid_vertices
	{
		{ glm::vec3(0.f, 0.f, 0.f),									{ 0.f, 1.f, 0.f}, { 0.f, 0.f } },
		{ glm::vec3(width, 0.f, 0.f),		{ 0.f, 1.f, 0.f}, { 0.f, 0.f } },
		{ glm::vec3(width, 0.f, depth),	{ 0.f, 1.f, 0.f}, { 0.f, 0.f } },
		{ glm::vec3(0.f, 0.f, depth),		{ 0.f, 1.f, 0.f}, { 0.f, 0.f } },
		{ glm::vec3(0.f, height, 0.f),		{ 0.f, 1.f, 0.f}, { 0.f, 0.f } },
		{ glm::vec3(width, height, 0.f),	{ 0.f, 1.f, 0.f}, { 0.f, 0.f } },
		{ glm::vec3(width, height, depth), { 0.f, 1.f, 0.f}, { 0.f, 0.f } },
		{ glm::vec3(0.f, height, depth),	{ 0.f, 1.f, 0.f}, { 0.f, 0.f } },
	};
	std::vector<uint32_t> grid_indices
	{
		0, 1,
		1, 2,
		2, 3,
		3, 0,

		4, 5,
		5, 6,
		6, 7,
		7, 4,

		0, 4,
		1, 5,
		2, 6,
		3, 7,
	};
	

	m_mesh = engine::mesh::create(grid_vertices, grid_indices);
}

void engine::bounding_box::on_update(glm::vec3 position, float rotation, glm::vec3 axis)
{
	m_position = position;
	m_rotation_amount = rotation;
	m_rotation_axis = axis;
}

void engine::bounding_box::on_render(float r, float g, float b, const engine::ref<engine::shader>& shader)
{
	glm::mat4 transform(1.0f);
	
	transform = glm::translate(transform, m_position);
	transform = glm::rotate(transform, m_rotation_amount, m_rotation_axis);
	transform = glm::translate(transform, glm::vec3(-m_width / 2.f, 0.f, -m_depth / 2.f));
	

	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("u_transform", transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("colouring_on", true);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("in_colour", glm::vec3(r, g, b));
	m_mesh->va()->bind();
	engine::renderer_api::draw_indexed_lines(m_mesh->va());
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("colouring_on", false);
}
