#include "health.h"
health::health(const engine::game_object_properties props) : engine::game_object(props)
{}
health::~health()
{}
void health::init()
{
	//Stores that the powerup is active
	m_is_active = true;
}
void health::update(glm::vec3 c, float dt)
{
	if (!m_is_active)
		return;
	//Sets the rotation so the cube is rotating
	set_rotation_amount(rotation_amount() + dt * 2.f);
}
engine::ref<health> health::create(const engine::game_object_properties& props)
{
	return std::make_shared<health>(props);
}
//Method to remove the health pack on pickup
void health::remove() {
	//Set active variable to false
	m_is_active = false;
}
//Getter for active state
bool health::getIsMActive() {
	return m_is_active;
}
