#pragma once
#include <engine.h>

class quad;

class controls
{
public:
	controls(const std::string& path, float width, float height);
	~controls();

	void on_update();
	void on_render(engine::ref<engine::shader> shader);
	//Functions to activate or deactivate the controls screen
	void activate();
	void deactivate();
	static engine::ref<controls> create(const std::string& path, float width, float height);

private:
	//Stores the active state of the controls screen
	bool s_active;
	engine::ref<engine::texture_2d> m_texture;
	engine::ref<quad> m_quad;

};
