#include "man.h"
man::man(const engine::game_object_properties props) : engine::game_object(props)
{}
man::~man()
{}
void man::init()
{
	//Stores the activate state of the man model
	m_is_active = true;
	s_active = false;
}
void man::update(glm::vec3 c, float dt)
{
	if (!s_active)
		return;
	//Sets the rotation on each update to keep it rotating
	set_rotation_amount(rotation_amount() + dt * 2.f);
}
//Activates the man model
void man::activate()
{
	s_active = true;
}
//Deactivates the man model
void man::deactivate() {
	s_active = false;
}
//Creates the man model
engine::ref<man> man::create(const engine::game_object_properties& props)
{
	return std::make_shared<man>(props);
}
