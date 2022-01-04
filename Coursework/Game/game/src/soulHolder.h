#pragma once
#include <engine.h>

class quad;

class soulHolder
{
public:
	soulHolder(const std::string& path, float width, float height);
	~soulHolder();

	void on_update();
	void on_render(engine::ref<engine::shader> shader);
	//Functions to activate or deactivate the soulHolder screen
	void activate();
	void deactivate();
	static engine::ref<soulHolder> create(const std::string& path, float width, float height);

private:
	//Stores the active state of the soulHolder screen
	bool s_active;
	engine::ref<engine::texture_2d> m_texture;
	engine::ref<quad> m_quad;
};
