#pragma once
#include <engine.h>

class quad;

class healthBar
{
public:
	healthBar(const std::string& path, float width, float height);
	~healthBar();

	void on_update();
	void on_render(engine::ref<engine::shader> shader);
	//Functions to activate or deactivate the healthBar screen
	void activate();
	void deactivate();
	static engine::ref<healthBar> create(const std::string& path, float width, float height);

private:
	//Stores the active state of the healthBar screen
	bool s_active;
	engine::ref<engine::texture_2d> m_texture;
	engine::ref<quad> m_quad;
};
