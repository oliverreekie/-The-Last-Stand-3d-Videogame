#pragma once
#include <engine.h>
class soul : public engine::game_object {
public:
	soul(const engine::game_object_properties props);
	~soul();
	void init();
	void update(glm::vec3 c, float dt);
	bool active() { return m_is_active; }
	static engine::ref<soul> create(const engine::game_object_properties& props);
	bool m_is_active = true;
	void remove();
	bool getIsMActive();
private:

};
