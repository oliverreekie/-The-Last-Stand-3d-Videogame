#pragma once
#include <engine.h>
class health : public engine::game_object {
public:
	health(const engine::game_object_properties props);
	~health();
	void init();
	void update(glm::vec3 c, float dt);
	bool active() { return m_is_active; }
	static engine::ref<health> create(const engine::game_object_properties& props);
	void remove();
	bool getIsMActive();
	bool m_is_active = true;
private:

};
