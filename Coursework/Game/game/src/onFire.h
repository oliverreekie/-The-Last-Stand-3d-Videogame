#pragma once
#include <engine.h>

class quad;

class onFire
{
public:
	onFire(const std::string& path, float width, float height);
	~onFire();
	//Functions to update and render the effect
	void on_update(const engine::timestep&);
	void on_render(engine::ref<engine::shader> shader);
	//Functions to activate or deactivate the onFire screen
	void activate();
	void deactivate();
	static engine::ref<onFire> create(const std::string& path, float width, float height);

	//Records whether to increase or decrease transparency
	bool increase = false;

private:
	//Stores the active state of the onFire screen
	bool s_active;
	engine::ref<engine::texture_2d> m_texture;
	engine::ref<quad> m_quad;
	//Stores current transparency
	float m_transparency;
	//Records current timer value
	float m_timer;
	//Holds the maximum effect time (10.f)
	float m_max_time;
};
