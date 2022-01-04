#include "soul.h"
soul::soul(const engine::game_object_properties props) : engine::game_object(props)
{}
soul::~soul()
{}
void soul::init()
{
	//Stores the active state of the powerup
	m_is_active = true;
}
void soul::update(glm::vec3 c, float dt)
{
	if (!m_is_active)
		return;
	//Sets the rotation so the soul is rotating
	set_rotation_amount(rotation_amount() + dt * 2.f);
}
engine::ref<soul> soul::create(const engine::game_object_properties& props)
{
	return std::make_shared<soul>(props);
}

//Removes the soul
void soul::remove() {
	m_is_active = false;
}

bool soul::getIsMActive() {
	return m_is_active;
}
