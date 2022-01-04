#pragma once
#include <engine.h>

class quad;

class died
{
public:
	died(const std::string& path, float width, float height);
	~died();

	void on_update(const engine::timestep& time_step);
	void on_render(engine::ref<engine::shader> shader);
	//Functions to activate or deactivate the died screen
	void activate();
	static engine::ref<died> create(const std::string& path, float width, float height);


private:
	//Stores the active state of the controls screen
	bool s_active;
	engine::ref<engine::texture_2d> m_texture;
	engine::ref<quad> m_quad;

	//Stores the transparency of the screen
	float transparency;
};
