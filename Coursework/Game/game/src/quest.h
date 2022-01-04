#pragma once
#include <engine.h>

class quad;

class quest
{
public:
	quest(const std::string& path, float width, float height);
	~quest();

	void on_update();
	void on_render(engine::ref<engine::shader> shader);
	//Functions to activate or deactivate the quest screen
	void activate();
	void deactivate();
	static engine::ref<quest> create(const std::string& path, float width, float height);

private:
	//Stores the active state of the quest screen
	bool s_active;
	engine::ref<engine::texture_2d> m_texture;
	engine::ref<quad> m_quad;
};
