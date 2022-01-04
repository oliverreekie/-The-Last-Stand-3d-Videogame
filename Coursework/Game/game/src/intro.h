#pragma once
#include <engine.h>

class quad;

class intro
{
public:
	intro(const std::string& path, float width, float height);
	~intro();

	void on_update(const engine::timestep& time_step);
	void on_render(engine::ref<engine::shader> shader);
	//Functions to activate or deactivate the intro screen
	void activate();
	void deactivate();
	static engine::ref<intro> create(const std::string& path, float width, float height);
	void startTheFade();

private:
	//Stores the active state of the controls screen
	bool s_active;
	engine::ref<engine::texture_2d> m_texture;
	engine::ref<quad> m_quad;

	//Stores the time remaining
	float remaining;

	//Stores the transparency
	float transparency;

	//Stores whether the fade has started
	bool startFade = false;
};
