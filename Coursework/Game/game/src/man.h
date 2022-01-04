#pragma once
#include <engine.h>
class man : public engine::game_object {
public:
	man(const engine::game_object_properties props);
	~man();
	void init();
	void update(glm::vec3 c, float dt);
	//Checks the active state of the man model
	bool active() { return m_is_active; }
	static engine::ref<man> create(const engine::game_object_properties& props);
	//Activates or deactivates the man model
	void activate();
	void deactivate();
	//Sets the active state of the man model
	bool getActive() {
		return s_active;
	}

private:
	//Stores the active state of the man model
	bool m_is_active;
	bool s_active;
};
