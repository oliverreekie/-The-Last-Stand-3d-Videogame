#include "example_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/utils/track.h"
#include "health.h"
#include "soul.h"
#include "engine/core/input.h"
#include "engine/key_codes.h"
#include <ioStream>
#include "man.h"

#include <GLFW/include/GLFW/glfw3.h>
#include "engine/utils/timer.h"


example_layer::example_layer() 
    :m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f), 
    m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height())
{
    // Hide the mouse and lock it inside the window
    //engine::input::anchor_mouse(true);
    engine::application::window().hide_mouse_cursor();

	// Initialise all audio sounds and play background music
	m_audio_manager = engine::audio_manager::instance();
	m_audio_manager->init();
	m_audio_manager->load_sound("assets/audio/battleMusic.mp3", engine::sound_type::track, "music");
	m_audio_manager->load_sound("assets/audio/laugh.mp3", engine::sound_type::event, "laugh");
	m_audio_manager->load_sound("assets/audio/zap.wav", engine::sound_type::event, "zap");
	m_audio_manager->load_sound("assets/audio/attack1.wav", engine::sound_type::event, "attack1");
	m_audio_manager->load_sound("assets/audio/attack2.wav", engine::sound_type::event, "attack2");
	m_audio_manager->load_sound("assets/audio/attack3.wav", engine::sound_type::event, "attack3");
	m_audio_manager->load_sound("assets/audio/ghost.wav", engine::sound_type::spatialised, "ghost");
	m_audio_manager->load_sound("assets/audio/summer.wav", engine::sound_type::track, "summer");
	m_audio_manager->play("music");


	// Initialise the shaders, materials and lights
	auto mesh_shader = engine::renderer::shaders_library()->get("mesh");
	auto text_shader = engine::renderer::shaders_library()->get("text_2D");

	//Initialise all values for each light in the game (excluding plasma lights)
	//The general directional light's intensity has been reduced to make the game darker
	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.4f;
	m_directionalLight.DiffuseIntensity = 0.8f;
	m_directionalLight.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));

	m_finalLight.Color = glm::vec3(1.0f, 0.88f, 0.68f);
	m_finalLight.AmbientIntensity = 1.0f;
	m_finalLight.DiffuseIntensity = 1.0f;
	m_finalLight.Position = glm::vec3(0.0f, 0.0f, 0.0f);

	m_spotLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_spotLight.AmbientIntensity = 2.25f;
	m_spotLight.DiffuseIntensity = 2.6f;
	m_spotLight.Position = glm::vec3(-21.2423, 3.4, 22.4219);
	m_spotLight.Direction = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
	m_spotLight.Cutoff = 0.5f;
	m_spotLight.Attenuation.Constant = 2.0f;
	m_spotLight.Attenuation.Linear = 0.1f;
	m_spotLight.Attenuation.Exp = 0.01f;


	// set color texture unit
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("lighting_on", true);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gColorMap", 0);
	m_directionalLight.submit(mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gMatSpecularIntensity", 1.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gSpecularPower", 10.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("transparency", 1.0f);

	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->set_uniform("projection",
		glm::ortho(0.f, (float)engine::application::window().width(), 0.f,
		(float)engine::application::window().height()));
	m_material = engine::material::create(1.0f, glm::vec3(1.0f, 0.1f, 0.07f),
		glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 0.0f);
	//Set material for mannequin
	m_mannequin_material = engine::material::create(1.0f,
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		1.0f);
	//Set material for diamond
	m_diamond_material = engine::material::create(32.0f,
		glm::vec3(1.0f, 0.5f, 0.0f),
		glm::vec3(1.0f, 0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		0.3f);
	//Set material for blade
	m_blade_material = engine::material::create(32.0f,
		glm::vec3(1.0f, 0.5f, 0.0f),
		glm::vec3(1.0f, 0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		0.3f);
	//Set material for hilt
	m_hilt_material = engine::material::create(32.0f,
		glm::vec3(1.0f, 0.5f, 0.0f),
		glm::vec3(1.0f, 0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		0.3f);
	//Set material for shield
	m_shield_material = engine::material::create(32.0f,
		glm::vec3(1.0f, 0.5f, 0.0f),
		glm::vec3(1.0f, 0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		0.3f);
	//Set material for trim
	m_trim_material = engine::material::create(32.0f,
		glm::vec3(1.0f, 0.5f, 0.0f),
		glm::vec3(1.0f, 0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		0.3f);
	//Set material for final light
	m_lightsource_material = engine::material::create(1.0f,
		m_finalLight.Color,
		m_finalLight.Color,
		m_finalLight.Color,
		1.0f);


	//Create the skybox txture
	m_skybox = engine::skybox::create(100.f,
		{ engine::texture_2d::create("assets/textures/gloomy_skybox/gloomy_rt.png", true),
		  engine::texture_2d::create("assets/textures/gloomy_skybox/gloomy_ft.png", true),
		  engine::texture_2d::create("assets/textures/gloomy_skybox/gloomy_lf.png", true),
		  engine::texture_2d::create("assets/textures/gloomy_skybox/gloomy_bk.png", true),
		  engine::texture_2d::create("assets/textures/gloomy_skybox/gloomy_up.png", true),
		  engine::texture_2d::create("assets/textures/gloomy_skybox/gloomy_dn.png", true)
		});

	//Set mesh for mannequin with each animation file
	engine::ref<engine::skinned_mesh> m_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/mannequin/free3Dmodel.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/walking.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/jump.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");
	m_skinned_mesh->switch_root_movement(false);

	//Sets position, scale, rotation axis and game object properties for the mannequin
	engine::game_object_properties mannequin_props;
	mannequin_props.animated_mesh = m_skinned_mesh;
	mannequin_props.scale = glm::vec3(1.f / glm::max(m_skinned_mesh->size().x, glm::max(m_skinned_mesh->size().y, m_skinned_mesh->size().z)));
	mannequin_props.position = glm::vec3(3.0f, 1.5f, -5.0f);
	mannequin_props.rotation_axis = glm::vec3(0.f, 1.f, 0.f);
	mannequin_props.type = 0;
	mannequin_props.bounding_shape = glm::vec3(m_skinned_mesh->size().x / 4.f, m_skinned_mesh->size().y / 2.f, m_skinned_mesh->size().x / 4.f);
	mannequin_props.friction = 0.0f;

	//Creates the mannequin with an offset
	m_mannequin = engine::game_object::create(mannequin_props);
	m_mannequin->set_offset(m_skinned_mesh->offset());

	//Initialises the player with the mannequin model and create the bounding box around it
	m_player.initialise(m_mannequin);
	m_player.set_box(mannequin_props.bounding_shape.x * 2.f * mannequin_props.scale.x, mannequin_props.bounding_shape.y * 2.f * mannequin_props.scale.x, mannequin_props.bounding_shape.z * 2.f * mannequin_props.scale.x, mannequin_props.position);


	//Creates dirt terrain texture and set up heightmap
	m_heightmap = engine::heightmap::create("assets/textures/heightmap.png", "assets/textures/dirt2.jpg", 100.f, 100.f, glm::vec3(0.f, 0.f, 0.f), 10.f);
	engine::game_object_properties terrain_props;
	terrain_props.meshes = { m_heightmap->mesh() };
	terrain_props.textures = { m_heightmap->texture() };
	terrain_props.is_static = true;
	terrain_props.type = 0;
	terrain_props.bounding_shape = glm::vec3(m_heightmap->terrain_size().x, m_physical_terrain_height, m_heightmap->terrain_size().y);
	terrain_props.restitution = 0.92f;
	m_terrain = engine::game_object::create(terrain_props);

	//Creates the ball used on victory condition
	engine::ref<engine::sphere> sphere_shape = engine::sphere::create(20, 40, 0.01f);
	engine::game_object_properties sphere_props;
	sphere_props.position = { 0.0f, 0.0f, 0.0f };
	sphere_props.meshes = { sphere_shape->mesh() };
	m_ball = engine::game_object::create(sphere_props);

	//Model for fallen warrior (scaled, rotated and positioned)
	dudePosition = glm::vec3(33.3f, 0.6f, -19.7f);
	engine::ref <engine::model> dude_model = engine::model::create("assets/models/static/Dude/Dude/test1.obj");
	engine::game_object_properties dude_props;
	dude_props.meshes = dude_model->meshes();
	dude_props.textures = dude_model->textures();
	float dude_scale = 1.f / glm::max(dude_model->size().x, glm::max(dude_model->size().y, dude_model->size().z));
	dude_props.position = { dudePosition };
	dude_props.scale = glm::vec3(dude_scale * 1.9f);
	dude_props.bounding_shape = dude_model->size() / 1.9f * dude_scale;
	m_dude = engine::game_object::create(dude_props);
	

	//Model for throne
	engine::ref <engine::model> throne_model = engine::model::create("assets/models/static/BirdmansChair/chair2.3ds");
	engine::game_object_properties throne_props;
	throne_props.meshes = throne_model->meshes();
	throne_props.textures = throne_model->textures();
	float throne_scale = 1.f / glm::max(throne_model->size().x, glm::max(throne_model->size().y, throne_model->size().z));
	throne_props.position = {-29.146f, 0.5f, 28.6725f};
	throne_props.scale = glm::vec3(throne_scale);
	throne_props.bounding_shape = throne_model->size() / 2.f * throne_scale;
	m_throne = engine::game_object::create(throne_props);

	//Model for cross
	engine::ref <engine::model> cross_model = engine::model::create("assets/models/static/Celtic/Cross.obj");
	engine::game_object_properties cross_props;
	cross_props.meshes = cross_model->meshes();
	cross_props.textures = cross_model->textures();
	float cross_scale = 1.f / glm::max(cross_model->size().x, glm::max(cross_model->size().y, cross_model->size().z));
	cross_props.position = { 33.f,0.6f, -40.f };
	cross_props.scale = glm::vec3(cross_scale);
	cross_props.bounding_shape = cross_model->size() / 4.5f * cross_scale;
	m_cross = engine::game_object::create(cross_props);

	//Creates the 22 blank balls for fire collision detection
	for (int i = 0; i < 22; i++) {
		glm::vec3 direction = glm::vec3(-4.9f, 0.5f, -2.14f);
		glm::vec3 newPos = glm::vec3(direction.x + (0.3 * i * 2), 0.5, direction.z + (0.3 * i * 2));
		//Method to create the blank ball, also is added to game object vector, see 'spawnBlankBalls()'
		spawnBlankBalls(newPos);
	}

	//Adds the terrain and player to game objects vector
	m_game_objects.push_back(m_terrain);
	m_game_objects.push_back(m_mannequin);
	m_physics_manager = engine::bullet_manager::create(m_game_objects);

	//Sets up the text manager
	m_text_manager = engine::text_manager::create();

	//Stores the initial mannequin animation
	m_skinned_mesh->switch_animation(1);

	//Diamond object to use for enemy soul
	//Adding the coordinates to make the diamond shape
	diamond_vertices.push_back(glm::vec3(0.f, 0.15f, 0.f));	//0
	diamond_vertices.push_back(glm::vec3(0.1f, 0.f, 0.1f));	//1
	diamond_vertices.push_back(glm::vec3(-0.1f, 0.f, 0.1f));	//2
	diamond_vertices.push_back(glm::vec3(-0.1f, 0.f, -0.1f));	//3
	diamond_vertices.push_back(glm::vec3(0.1f, 0.0f, -0.1f));	//4
	diamond_vertices.push_back(glm::vec3(0.0f, -0.15f, 0.0f));	//5

	//Blade
	//Adding the coordinates to make the blade shape
	std::vector<glm::vec3> blade_vertices;
	blade_vertices.push_back(glm::vec3(0.5f, 0.15f, 1.7f));	//0
	blade_vertices.push_back(glm::vec3(0.4f, 0.12f, 1.2f));	//1
	blade_vertices.push_back(glm::vec3(0.5f, 0.15f, 1.2f));	//2
	blade_vertices.push_back(glm::vec3(0.6f, 0.12f, 1.2f));	//3

	blade_vertices.push_back(glm::vec3(0.4f, 0.12f, -0.3));	//4
	blade_vertices.push_back(glm::vec3(0.5f, 0.15f, -0.3));	//5
	blade_vertices.push_back(glm::vec3(0.6f, 0.12f, -0.3f));//6

	blade_vertices.push_back(glm::vec3(0.5f, 0.09f, 1.2f));	//7
	blade_vertices.push_back(glm::vec3(0.5f, 0.09f, -0.3f));//8

	//Creates the blade shape and textures with metal texture
	engine::ref<engine::blade> blade_shape = engine::blade::create(blade_vertices);
	std::vector<engine::ref<engine::texture_2d>> blade_textures = { engine::texture_2d::create("assets/textures/blade.jpg", false) };
	engine::game_object_properties blade_props;
	blade_props.position = { 0.f, -12.0f, 0.f };
	blade_props.meshes = { blade_shape->mesh() };
	blade_props.textures = blade_textures;
	m_blade = engine::game_object::create(blade_props);


	//Hilt
	//Adding the coordinates to make the hilt shape
	std::vector<glm::vec3> hilt_vertices;

	//Guard front face
	hilt_vertices.push_back(glm::vec3(0.15f, 0.15f, -0.3f));	//0
	hilt_vertices.push_back(glm::vec3(0.15f, 0.15f, -0.45f));	//1
	hilt_vertices.push_back(glm::vec3(0.85f, 0.15f, -0.3f));	//2
	hilt_vertices.push_back(glm::vec3(0.85f, 0.15f, -0.45f));	//3

	//Guard back face
	hilt_vertices.push_back(glm::vec3(0.15f, 0.09f, -0.3f));	//4
	hilt_vertices.push_back(glm::vec3(0.15f, 0.09f, -0.45f));	//5
	hilt_vertices.push_back(glm::vec3(0.85f, 0.09f, -0.3f));	//6
	hilt_vertices.push_back(glm::vec3(0.85f, 0.09f, -0.45f));	//7

	//Holder front face
	hilt_vertices.push_back(glm::vec3(0.42f, 0.15f, -0.45f));	//8
	hilt_vertices.push_back(glm::vec3(0.42f, 0.15f, -0.9f));	//9
	hilt_vertices.push_back(glm::vec3(0.58f, 0.15f, -0.45f));	//10
	hilt_vertices.push_back(glm::vec3(0.58f, 0.15f, -0.9f));	//11

	//Holder back face
	hilt_vertices.push_back(glm::vec3(0.42f, 0.09f, -0.45f));	//12
	hilt_vertices.push_back(glm::vec3(0.42f, 0.09f, -0.9f));	//13
	hilt_vertices.push_back(glm::vec3(0.58f, 0.09f, -0.45f));	//14
	hilt_vertices.push_back(glm::vec3(0.58f, 0.09f, -0.9f));	//15

	//Creates the hilt shape and textures with gold texture
	engine::ref<engine::hilt> hilt_shape = engine::hilt::create(hilt_vertices);
	std::vector<engine::ref<engine::texture_2d>> hilt_textures = { engine::texture_2d::create("assets/textures/gold.jpg", false) };
	engine::game_object_properties hilt_props;
	hilt_props.position = { 0.f, -12.0f, 0.f };
	hilt_props.meshes = { hilt_shape->mesh() };
	hilt_props.textures = hilt_textures;
	m_hilt = engine::game_object::create(hilt_props);

	//Shield
	//Adding the coordinates to make the shield shape
	shield_vertices.push_back(glm::vec3(0.01f, 1.5f, 0.20f));//0
	shield_vertices.push_back(glm::vec3(0.13f, 1.5f, 0.20f));//1
	shield_vertices.push_back(glm::vec3(0.13f, 1.5f, 0.09f));//2
	shield_vertices.push_back(glm::vec3(0.12f, 1.5f, 0.07f));//3

	shield_vertices.push_back(glm::vec3(0.108f, 1.5f, 0.051f));//4
	shield_vertices.push_back(glm::vec3(0.093f, 1.5f, 0.035f));//5
	shield_vertices.push_back(glm::vec3(0.07f, 1.5f, 0.02f));//6
	shield_vertices.push_back(glm::vec3(0.047f, 1.5f, 0.035f));//7

	shield_vertices.push_back(glm::vec3(0.032f, 1.5f, 0.051f));//8
	shield_vertices.push_back(glm::vec3(0.02f, 1.5f, 0.07f));//9
	shield_vertices.push_back(glm::vec3(0.01f, 1.5f, 0.09f));//10
	shield_vertices.push_back(glm::vec3(0.07f, 1.5f, 0.09f));//11

	shield_vertices.push_back(glm::vec3(0.07f, 1.5f, 0.20f));//12
	shield_vertices.push_back(glm::vec3(0.01f, 1.48f, 0.20f));//13
	shield_vertices.push_back(glm::vec3(0.13f, 1.48f, 0.20f));//14
	shield_vertices.push_back(glm::vec3(0.13f, 1.48f, 0.09f));//15

	shield_vertices.push_back(glm::vec3(0.12f, 1.48f, 0.07f));//16
	shield_vertices.push_back(glm::vec3(0.108f, 1.48f, 0.051f));//17
	shield_vertices.push_back(glm::vec3(0.093f, 1.48f, 0.035f));//18
	shield_vertices.push_back(glm::vec3(0.07f, 1.48f, 0.02f));//19

	shield_vertices.push_back(glm::vec3(0.047f, 1.48f, 0.035f));//20
	shield_vertices.push_back(glm::vec3(0.032f, 1.48f, 0.051f));//21
	shield_vertices.push_back(glm::vec3(0.02f, 1.48f, 0.07f));//22
	shield_vertices.push_back(glm::vec3(0.01f, 1.48f, 0.09f));//23

	shield_vertices.push_back(glm::vec3(0.07f, 1.48f, 0.09f));//24
	shield_vertices.push_back(glm::vec3(0.07f, 1.48f, 0.20f));//25

	//Creates the shield shape and textures with wood texture
	engine::ref<engine::shield> shield_shape = engine::shield::create(shield_vertices);
	std::vector<engine::ref<engine::texture_2d>> shield_textures = { engine::texture_2d::create("assets/textures/wood.jpg", false) };
	engine::game_object_properties shield_props;
	shield_props.position = { 0.f, -12.0f, 0.f };
	shield_props.meshes = { shield_shape->mesh() };
	shield_props.textures = shield_textures;
	m_shield = engine::game_object::create(shield_props);

	//Trim
	//Adding the coordinates to make the trim shape
	std::vector<glm::vec3> trim_vertices;
	trim_vertices.push_back(glm::vec3(0.06f, 1.501f, 0.20f));	//0
	trim_vertices.push_back(glm::vec3(0.07f, 1.501f, 0.20f));	//1
	trim_vertices.push_back(glm::vec3(0.08f, 1.501f, 0.20f));	//2
	trim_vertices.push_back(glm::vec3(0.08f, 1.501f, 0.027));	//3

	trim_vertices.push_back(glm::vec3(0.07f, 1.501f, 0.02f));	//4
	trim_vertices.push_back(glm::vec3(0.06f, 1.501f, 0.027f));	//5
	trim_vertices.push_back(glm::vec3(0.13f, 1.501f, 0.12f));	//6
	trim_vertices.push_back(glm::vec3(0.13f, 1.501f, 0.10f));	//7

	trim_vertices.push_back(glm::vec3(0.07f, 1.501f, 0.10f));	//8
	trim_vertices.push_back(glm::vec3(0.07f, 1.501f, 0.12f));	//9
	trim_vertices.push_back(glm::vec3(0.01f, 1.501f, 0.10f));	//10
	trim_vertices.push_back(glm::vec3(0.01f, 1.501f, 0.12f));	//11

	//Creates the trim shape and textures with gold texture (cross design on shield)
	engine::ref<engine::trim> trim_shape = engine::trim::create(trim_vertices);
	std::vector<engine::ref<engine::texture_2d>> trim_textures = { engine::texture_2d::create("assets/textures/gold.jpg", false) };
	engine::game_object_properties trim_props;
	trim_props.position = { 0.f, -12.0f, 0.f };
	trim_props.meshes = { trim_shape->mesh() };
	trim_props.textures = trim_textures;
	m_trim = engine::game_object::create(trim_props);

	//Creates m_plasma object
	m_plasma = plasma::create(glm::vec3(1.f, 0.f, 1.0f), glm::vec3(0.f, 0.f, 1.f), 0.25f, 3.0f);

	//Creating the intro, control and quest screens
	m_intro = intro::create("assets/textures/intro.png", 1.6f, 0.9f);
	m_controls = controls::create("assets/textures/controls.png", 1.6f, 0.9f);
	m_quest = quest::create("assets/textures/quest.png", 1.6f, 0.9f);

	//Loads texture for flame particle effect
	theFlame.load_texture("assets/textures/flame.png");

	//Loads texture for 'on fire' effect
	m_on_fire = onFire::create("assets/textures/fire.png", 1.6f, 0.9f);

	//Loads texture for death screen
	m_died = died::create("assets/textures/died.png", 1.6f, 0.9f);

	//Loads texture for lose screen
	m_lose = died::create("assets/textures/lose.png", 1.6f, 0.9f);

	//Loads texture for each of the soul holder states
	m_soulHolder0 = soulHolder::create("assets/textures/HUDSouls/souls0.png", 0.25f, 0.3f);
	m_soulHolder1 = soulHolder::create("assets/textures/HUDSouls/souls1.png", 0.25f, 0.3f);
	m_soulHolder2 = soulHolder::create("assets/textures/HUDSouls/souls2.png", 0.25f, 0.3f);
	m_soulHolder3 = soulHolder::create("assets/textures/HUDSouls/souls3.png", 0.25f, 0.3f);
	m_soulHolder4 = soulHolder::create("assets/textures/HUDSouls/souls4.png", 0.25f, 0.3f);
	m_soulHolder5 = soulHolder::create("assets/textures/HUDSouls/souls5.png", 0.25f, 0.3f);
	m_soulHolder6 = soulHolder::create("assets/textures/HUDSouls/souls6.png", 0.25f, 0.3f);
	m_soulHolder7 = soulHolder::create("assets/textures/HUDSouls/souls7.png", 0.25f, 0.3f);

	//Loads texture for each of the health bar states
	m_healthBar0 = healthBar::create("assets/textures/HUDHealth/health0.png", 0.4f, 0.10f);
	m_healthBar5 = healthBar::create("assets/textures/HUDHealth/health5.png", 0.4f, 0.10f);
	m_healthBar10 = healthBar::create("assets/textures/HUDHealth/health10.png", 0.4f, 0.10f);
	m_healthBar15 = healthBar::create("assets/textures/HUDHealth/health15.png", 0.4f, 0.10f);
	m_healthBar20 = healthBar::create("assets/textures/HUDHealth/health20.png", 0.4f, 0.10f);
	m_healthBar25 = healthBar::create("assets/textures/HUDHealth/health25.png", 0.4f, 0.10f);
	m_healthBar30 = healthBar::create("assets/textures/HUDHealth/health30.png", 0.4f, 0.10f);
	m_healthBar35 = healthBar::create("assets/textures/HUDHealth/health35.png", 0.4f, 0.10f);
	m_healthBar40 = healthBar::create("assets/textures/HUDHealth/health40.png", 0.4f, 0.10f);
	m_healthBar45 = healthBar::create("assets/textures/HUDHealth/health45.png", 0.4f, 0.10f);
	m_healthBar50 = healthBar::create("assets/textures/HUDHealth/health50.png", 0.4f, 0.10f);
	m_healthBar55 = healthBar::create("assets/textures/HUDHealth/health55.png", 0.4f, 0.10f);
	m_healthBar60 = healthBar::create("assets/textures/HUDHealth/health60.png", 0.4f, 0.10f);
	m_healthBar65 = healthBar::create("assets/textures/HUDHealth/health65.png", 0.4f, 0.10f);
	m_healthBar70 = healthBar::create("assets/textures/HUDHealth/health70.png", 0.4f, 0.10f);
	m_healthBar75 = healthBar::create("assets/textures/HUDHealth/health75.png", 0.4f, 0.10f);
	m_healthBar80 = healthBar::create("assets/textures/HUDHealth/health80.png", 0.4f, 0.10f);
	m_healthBar85 = healthBar::create("assets/textures/HUDHealth/health85.png", 0.4f, 0.10f);
	m_healthBar90 = healthBar::create("assets/textures/HUDHealth/health90.png", 0.4f, 0.10f);
	m_healthBar95 = healthBar::create("assets/textures/HUDHealth/health95.png", 0.4f, 0.10f);
	m_healthBar100 = healthBar::create("assets/textures/HUDHealth/health100.png", 0.4f, 0.10f);

	//Model for rotating mannequin on controls screen (positioned and scaled to fit)
	engine::ref <engine::model> man_model = engine::model::create("assets/models/animated/mannequin/free3Dmodel.dae");
	engine::game_object_properties man_props;
	man_props.meshes = man_model->meshes();
	man_props.textures = man_model->textures();
	float man_scale = 1.f / glm::max(man_model->size().x * 1.2f, glm::max(man_model->size().y * 1.2f, man_model->size().z * 1.2f));
	man_props.position = { -0.45f, -0.5f, 0.0f };
	man_props.scale = glm::vec3(man_scale);
	man_props.bounding_shape = man_model->size() / 1.0f * man_scale;
	m_man = man::create(man_props);

	//Activate controls screen and man model on start up
	m_controls->activate();
	m_man->activate();

	//Set player to initally be standing still in the still animation
	m_player.setStill();
	m_player.setWalk(false);
	m_player.setRunning(false);

	//Hides the quest during the intro screen
	hideQuest = true;

	//Sets the initial quest number to 0
	questNumber = 0;

	//The number of minutes on the timer. Currently at 6 minutes (in miliseconds)
	counter = 360000;

	//Spawns the initial 3 health packs
	spawnHealthPack();
	spawnHealthPack();
	spawnHealthPack();

	//Spawns the initial 2 boss area health packs
	spawnBossHealthPack();
	spawnBossHealthPack();

	//Initialises the flame particle effect
	theFlame.initialise(glm::vec3(7.31485, 0.5, 9.75924), glm::vec3(-12.2f, 0.f, -11.85f));

	//Starts the spell circle effect
	m_spell_circle.initialise();
	m_spell_circle.activate(glm::vec3(-21.2423, 0.5, 22.4219));

	//Spawns the boss model in front of his throne
	bossSpawn(glm::vec3(-26.4f, 1.f, 26.5f));

	//Spawns the initial 6 rat enemies at various positions
	ratSpawn(glm::vec3(38.9f, 1.f, -20.9f));
	ratSpawn(glm::vec3(16.7f, 1.f, -15.5f));
	ratSpawn(glm::vec3(-10.7f, 1.f, -17.8f));
	ratSpawn(glm::vec3(-28.1f, 1.f, -29.8f));
	ratSpawn(glm::vec3(18.4f, 1.f, 9.5f));
	ratSpawn(glm::vec3(37.5f, 1.f, 25.1f));

}

example_layer::~example_layer() {}

void example_layer::on_update(const engine::timestep& time_step)
{
	//Updates the camera
	m_3d_camera.on_update(time_step);

	//Updates the physics manager
	m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));

	//Updates the audio manager with camera position
	m_audio_manager->update_with_camera(m_3d_camera);

	//Plays the ghost sound at the spell circle on a loop every 6 seconds unless the player wins the game
	//Uses a timer taking into account frame rate to do this
	if (hasVictory == false) {
		if (ghostCounter <= 0.0f) {
			m_audio_manager->play_spatialised_sound("ghost", m_3d_camera.position(), glm::vec3(-21.2423, 0.5, 22.4219));
			ghostCounter = 6.0f;
		}
		else {
			ghostCounter -= 1.0f * time_step;
		}
	}

	//Updates the spell circle
	m_spell_circle.on_update(time_step);

	//Updates the rotation for each of the health packs
	for (int i = 0; i < storingHealth.size(); i++) {
		storingHealth[i]->set_rotation_amount(storingHealth[i]->rotation_amount() + time_step * 2.0f);
	}
	for (int i = 0; i < storingBossHealth.size(); i++) {
		storingBossHealth[i]->set_rotation_amount(storingBossHealth[i]->rotation_amount() + time_step * 2.0f);
	}

	//Updates the rotation for each of the souls
	for (int i = 0; i < storingSouls.size(); i++) {
		storingSouls[i]->set_rotation_amount(storingSouls[i]->rotation_amount() + time_step * 2.0f);
	}

	//Updates the player
	m_player.on_update(time_step);

	//Function to check which camera type to show (comment out for devMode)
	minimap();

	//Updates all information screens
	m_intro->on_update(time_step);
	m_controls->on_update();
	m_quest->on_update();
	m_on_fire->on_update(time_step);
	m_lose->on_update(time_step);
	m_died->on_update(time_step);

	//Updates the current soul holder state
	m_soulHolder0->on_update();
	m_soulHolder1->on_update();
	m_soulHolder2->on_update();
	m_soulHolder3->on_update();
	m_soulHolder4->on_update();
	m_soulHolder5->on_update();
	m_soulHolder6->on_update();
	m_soulHolder7->on_update();

	//Updates the current health bar state
	updateHealthBars();

	//Keep the rotating man on the info screen
	m_man->update(m_3d_camera.position(), time_step);

	//Function to give the first quest to the player when they approach the fallen warrior
	if (hasQuest1 == false) {
		//Calculates the distance between the player and the warrior
		warriorLength = m_player.getThePosition() - dudePosition;
		//If the player is close to the warrior
		if (glm::length(warriorLength) < 1.5) {
			//Show the quest screen
			setIsQuest(true);
			//Clear quest from the hud while the quest screen is open
			info = "";
			//Stop player motion and set animation to still for when quest screen closes
			m_player.set_m_speed(0.0f);
			m_player.setAnimation(2);
			m_player.setWalk(false);
			//Open the first quest screen
			m_quest->activate();
			//Set the current quest to 1
			questNumber = 1;
			//Stores that the user is working on quest number 1
			hasQuest1 = true;
			//Hide the quest
			hideQuest = true;
		}
	}
	//If the quest is able to be shown
	if (hideQuest == false) {
		//Show the initial quest to find the soldier
		if (questNumber == 0) {
			info = "Find one of your soldiers to figure out what has happened";
		}
		//Shows the second quest to collect souls after speaking to the soldier
		else if (questNumber == 1) {
			info = "Collect 7 souls from monsters";
		}
		//Shows the third quest to cross the flames into the boss area
		else if (questNumber == 2) {
			info = "Cross the cursed flames";
		}
		//Shows the fourth quest to slay the evil sorcerer
		else if (questNumber == 3) {
			info = "Slay the evil sorcerer";
		}
	}
	else {
		info = "";
	}

	//Sets quest number to three when the player first enters the boss area
	if (hasQuest4Shown == false) {
		if (m_player.getThePosition().z >= 4.8f && m_player.getThePosition().x <= 0.0f) {
			questNumber = 3;
			hasQuest4Shown = true;
		}
	}

	//Shows the current amount of souls on HUD
	if (hasQuest1 == true) {
		//Deactivates all the soul holder states
		m_soulHolder0->deactivate();
		m_soulHolder1->deactivate();
		m_soulHolder2->deactivate();
		m_soulHolder3->deactivate();
		m_soulHolder4->deactivate();
		m_soulHolder5->deactivate();
		m_soulHolder6->deactivate();
		m_soulHolder7->deactivate();
		//Activates the relevant state
		if (soulAmount == 0) {
			m_soulHolder0->activate();
		}
		else if (soulAmount == 1) {
			m_soulHolder1->activate();
		}
		else if (soulAmount == 2) {
			m_soulHolder2->activate();
		}
		else if (soulAmount == 2) {
			m_soulHolder2->activate();
		}
		else if (soulAmount == 3) {
			m_soulHolder3->activate();
		}
		else if (soulAmount == 4) {
			m_soulHolder4->activate();
		}
		else if (soulAmount == 5) {
			m_soulHolder5->activate();
		}
		else if (soulAmount == 6) {
			m_soulHolder6->activate();
		}
		else if (soulAmount >= 7) {
			m_soulHolder7->activate();
		}
	}

	//Set the current health bar state based on current health
	setHealthBarState();

	//Start the main game timer
	if (startMainTimer == true) {
		theTimer(time_step);
	}
	//If boss is active
	if (startBossMethods == true) {
		//For each update cycle, if the boss sets its shoot method to active, spawn a boss plasma and boss plasma ball and shoot at the player
		if (theBoss.getShoot() == true) {
			spawnBossPlasma(glm::vec3(m_boss->position().x + (theBoss.getShootingDirection().x / 2), 1.0f, m_boss->position().z + (theBoss.getShootingDirection().z / 2)));
			spawnBossPlasmaBall(glm::vec3(m_boss->position().x + (theBoss.getShootingDirection().x / 2), 1.0f, m_boss->position().z + (theBoss.getShootingDirection().z / 2)));
			theBoss.setShoot(false);
		}
	}
	//Update m_plasma
	m_plasma->on_update(time_step);

	//For all the plasma balls
	for (int i = 0; i < storingPlasma.size(); i++) {
		//Update plasma ball
		storingPlasma[i]->on_update(time_step);
		//If it has been released, move towards the player slightly and store positions
		if (storingPlasma[i]->getIsReleased() == true) {
			glm::vec3 newPos = lastPlasmaPosition[i] + (m_player.getDirection() / 2.f);
			storingPlasma[i]->setPosition(newPos);
			lastPlasmaPosition[i] = newPos;
		}
		//If it has not been released, update position to in front of the player so the player carries it around
		else {
			storingPlasma[i]->setPosition(glm::vec3(m_player.getThePosition().x + (m_player.getDirection().x / 2), 1.0f, m_player.getThePosition().z + (m_player.getDirection().z / 2)));
			lastPlasmaPosition[i] = (glm::vec3(m_player.getThePosition().x + (m_player.getDirection().x / 2), 1.0f, m_player.getThePosition().z + (m_player.getDirection().z / 2)));
		}
	}

	//For all of the plasma balls
	for (int i = 0; i < storingPlasmaBalls.size(); i++) {
		//If the time remaning is greater than zero
		if (plasmaBallTimeRemaining[i] > 0.0f) {
			//Move towards player slightly and store positions
			glm::vec3 newPos = lastPlasmaBallPosition[i] + (m_player.getDirection() / 2.f);
			storingPlasmaBalls[i]->set_position(newPos);
			lastPlasmaBallPosition[i] = newPos;
			//Decrease timer for plasma ball
			plasmaBallTimeRemaining[i] -= 2.0 * time_step;
		}
		//If plasma ball time is less than zero
		else {
			//Store that the plasma can be reused
			availablePlasmaBall[i] = true;
			//Set position to below terrain
			storingPlasmaBalls[i]->set_position(glm::vec3(0.0f, -30.0f, 0.0f));
		}
	}

	//For all of the boss plasma balls
	for (int i = 0; i < storingBossPlasma.size(); i++) {
		//Update boss plasma
		storingBossPlasma[i]->on_update(time_step);
		//Set boss plasma to move towards the player and store the position values
		glm::vec3 newPos = lastBossPlasmaPosition[i] + (theBoss.getShootingDirection() / 2.f);
		storingBossPlasma[i]->setPosition(newPos);
		lastBossPlasmaPosition[i] = newPos;
		storingBossPlasmaBalls[i]->set_position(newPos);

	}

	//For all the plasma lights
	for (int i = 0; i < storingPlasmaLights.size(); i++) {
		//If the timer reamining is greater than zero
		if (plasmaTimeRemaining[i] > 0.0f) {
			//Move towards the player slightly and store position values
			glm::vec3 newPos = lastPlasmaLightPosition[i] + (m_player.getDirection() / 2.f);
			storingPlasmaLights[i]->Position = newPos;
			lastPlasmaLightPosition[i] = newPos;
			//Decrease the time remaining
			plasmaTimeRemaining[i] -= 2.0 * time_step;
		}
		//If time remaining is less than zero
		else {
			//Set colour to black (i.e. make invisible)
			storingPlasmaLights[i]->Color = (glm::vec3(0.0f, 0.0f, 0.0f));
			//Record tha plasma light can be reused
			availablePlasma[i] = true;
		}
	}

	//Check if health pack has been hit
	for (int i = 0; i < storingHealth.size(); i++) {
		//Calculate the distance between the player and the health pack
		glm::vec3 theDistance = storingHealth[i]->position() - m_player.getThePosition();
		//If player reaches health pack
		if (glm::length(theDistance) < 0.7f) {
			//Remove health pack
			storingHealth[i]->remove();
			storingHealth.erase(storingHealth.begin() + i);
			//Increase player health by 20
			m_player.increaseHealth(20);
			//If player health is now greater than 100 (the maximum), set to 100
			if (m_player.getHealth() > 100) {
				m_player.setHealth(100);
			}
			//Spawn a new health pack
			spawnHealthPack();
		}
	}

	//For all of the boss area health packs
	for (int i = 0; i < storingBossHealth.size(); i++) {
		//Tells the boss class where the health packs are
		if (bossHasSpawned) {
			if (i == 0) {
				theBoss.setHealthPack1Pos(storingBossHealth[i]->position());
			}
			else {
				theBoss.setHealthPack2Pos(storingBossHealth[i]->position());
			}
		}
		//If the player hits the boss health pack
		glm::vec3 theDistance = storingBossHealth[i]->position() - m_player.getThePosition();
		if (glm::length(theDistance) < 0.7f) {
			//Remove boss health pack
			storingBossHealth[i]->remove();
			storingBossHealth.erase(storingBossHealth.begin() + i);
			//Increase player health by 20
			m_player.increaseHealth(20);
			if (m_player.getHealth() > 100) {
				m_player.setHealth(100);
			}
			//Spawn new boss health pack
			spawnBossHealthPack();
		}
		//If the boss hits the boss health pack
		glm::vec3 bossDistance = storingBossHealth[i]->position() - m_boss->position();
		//If boss hits the boss health pack
		if (glm::length(bossDistance) < 1.0f) {
			//Remove boss health pack
			storingBossHealth[i]->remove();
			storingBossHealth.erase(storingBossHealth.begin() + i);
			//Spawn new boss health pack
			spawnBossHealthPack();
		}
	}

	//Check if soul has been hit
	for (int i = 0; i < storingSouls.size(); i++) {
		//If player hits the soul
		glm::vec3 theDistance = storingSouls[i]->position() - m_player.getThePosition();
		if (glm::length(theDistance) < 0.7f) {
			//Remove the soul
			storingSouls[i]->remove();
			storingSouls.erase(storingSouls.begin() + i);
			soulTimer.erase(soulTimer.begin() + i);
			//If player has quest 1
			if (hasQuest1) {
				//Increase the number of souls collected
				soulAmount += 1;
				//If player has more than 7 souls
				if (soulAmount == 7) {
					//Progress to the next quest
					questNumber += 1;
					//Remove the blank balls for fire collision
					for (int j = 0; j < 22; j++) {
						m_game_objects.erase(m_game_objects.begin());
					}
					//Activate the boss and stop rats spawning
					startBossMethods = true;
					ratCanSpawn = false;
				}
			}
		}
	}

	//Updates the flame effect
	theFlame.on_update(time_step);

	//If player is on fire, continue to player fire effect
	if (fireTrigger == true) {
		fireHit(time_step);
	}

	//Reduce the time remaining for each soul
	reduceDiamondTimes(time_step);

	//If the player wins the game, grow the light ball in the middle of the screen
	if (showFinalBall == true) {
		growBall(time_step);
	}

	//If the player tries to walk on a surface that is too high or low (i.e. the cliffs around the map)
	if (m_heightmap->ground_height(m_player.getThePosition()) >= 0.47 || m_heightmap->ground_height(m_player.getThePosition()) <= 0.20) {
		//Stop player walking and sets animation to still
		m_player.set_m_speed(0.0f);
		m_player.setRunning(false);
		m_player.setWalk(false);
		m_player.setAnimation(2);
	}

	//If spell circle has grown big enough
	if (m_spell_circle.getRadius() >= 74.0) {
		if (isDefeatLaughing == false) {
			//Activate the lose screen
			m_lose->activate();
			//Removes timer and quest
			hideQuest = true;
			showMainTimer = false;
			//Plays laughing track
			m_audio_manager->pause("music");
			m_audio_manager->play("laugh");
			//Causes the laugh to only run once
			isDefeatLaughing = true;
		}
	}

	//If player health is less than or equal to zero
	if (m_player.getHealth() <= 0.0f) {
		if (isLaughing == false) {
			//Stops player moving
			m_player.set_m_speed(0.0f);
			//Change animation to still
			m_player.setAnimation(2);
			//Record this state
			m_player.setWalk(false);
			m_player.setRunning(false);
			//Rotates player to fall on the floor
			m_player.startDeath();
			//Show death screen
			m_died->activate();
			//Removes timer and quest
			showMainTimer = false;
			hideQuest = true;
			//Plays laughing track
			m_audio_manager->pause("music");
			m_audio_manager->play("laugh");
			//Causes the laugh to only run once
			isLaughing = true;
		}
	}

	//Fire collision
	//If less than 7 souls have been collected
	if (soulAmount < 7) {
		//Get collisions for the blank fire balls
		std::vector<engine::ref<engine::game_object>> theCollisions{};
		theCollisions = m_player.object()->collision_objects();
		//For each of the blank fire balls
		for (int i = 0; i < storingBlankBalls.size(); i++) {
			if (std::count(theCollisions.begin(), theCollisions.end(), storingBlankBalls[i])) {
				//Change animation to still
				m_player.setAnimation(2);
				//Record this state
				m_player.setWalk(false);
				m_player.setRunning(false);
				//Set player on fire
				if (whileOnFire == false) {
					setOnFire();
				}
				//Sets player velocity to shoot player back and into the air
				m_player.object()->set_velocity(glm::vec3(15.0f, 0.6f, -15.0f) * 12.f);
				break;
			}
		}
	}

	//For all of the plasma balls
	for (int i = 0; i < storingPlasmaBalls.size(); i++) {
		//If the plasma ball can affect (to ensure it only causes a collision once)
		if (plasmaCanAffect[i] == true) {
			//Gets the plasma collisions
			std::vector<engine::ref<engine::game_object>> plasmaCollisions{};
			plasmaCollisions = storingPlasmaBalls[i]->collision_objects();
			//If plasma hits a blank ball, stops the collision ability
			for (int j = 0; j < storingBlankBalls.size(); j++) {
				if (std::count(plasmaCollisions.begin(), plasmaCollisions.end(), storingBlankBalls[j])) {
					plasmaCanAffect[i] = false;
				}
			}
			//If plasma hits the boss
			if (bossHasSpawned == true) {
				if (std::count(plasmaCollisions.begin(), plasmaCollisions.end(), theBoss.object())) {
					//Decrease the boss health
					theBoss.decreaseHealth(10);
					plasmaCanAffect[i] = false;
				}

			}
			//If plasma hits a rat
			for (int j = 0; j < storingRats.size(); j++) {
				if (std::count(plasmaCollisions.begin(), plasmaCollisions.end(), storingRats[j].object())) {
					if (hasVictory == false) {
						//Starts rat curious method
						storingRats[j].setIsCurious(true);
						storingRats[j].setCuriousGoal(m_player.getThePosition());
						//Decrease rat health
						storingRats[j].decreaseHealth(15);
						plasmaCanAffect[i] = false;
					}

				}
			}
		}
	}

	//Collisions with the player
	std::vector<engine::ref<engine::game_object>> playerCollisions{};
	playerCollisions = m_player.object()->collision_objects();
	//If victory has begun
	if (hasVictory == false && showFinalBall == false) {
		//For all the rats
		for (int i = 0; i < storingRats.size(); i++) {
			//If player is alive
			if (m_player.getDeath() == false) {
				//If the rat is able to attack the player (so the player doesn't die too easily)
				if (ratDamageCounter <= 0.0f) {
					if (std::count(playerCollisions.begin(), playerCollisions.end(), storingRats[i].object())) {
						//Sets rat damage counter
						ratDamageCounter = 0.25f;
						//Reduce player health by 15
						m_player.reduceHealth(15);
						//Generates a random number between 1 and 3
						int num1 = rand() % 3;
						std::cout << (num1) << "\n";
						//Play randomly generated attack sound
						if (num1 == 0) {
							m_audio_manager->play("attack1");
						}
						else if (num1 == 1) {
							m_audio_manager->play("attack2");
						}
						else {
							m_audio_manager->play("attack3");
						}

					}
				}
			}
		}
	}

	//Reduce rat damage counter over time
	ratDamageCounter -= 0.25f * time_step;

	//For all of the boss plasma balls
	for (int i = 0; i < storingBossPlasmaBalls.size(); i++) {
		//If player has not been defeated
		if(hasDefeat == false){
			//If boss plasma is able to affect
			if (bossPlasmaCanAffect[i] == true) {
				//For all plasma collisions
				std::vector<engine::ref<engine::game_object>> plasmaCollisions{};
				plasmaCollisions = storingBossPlasmaBalls[i]->collision_objects();
				//If boss plasma ball hits player
				if (std::count(plasmaCollisions.begin(), plasmaCollisions.end(), m_player.object())){
					//Reduce player health by 6
					m_player.reduceHealth(6);
					//Stops boss plasma from causing collision
					bossPlasmaCanAffect[i] = false;
				}
			}
		}
	}

	//If boss has spawned, update boss
	if (bossHasSpawned == true) {
		theBoss.on_update(time_step, m_player.getThePosition(), startBossMethods);
	}

	//If the boss is killed start victory
	if (defeatStarted == false) {
		if (theBoss.getIsKilled() == true) {
			showFinalBall = true;
			defeatStarted = true;
		}
	}

	//For all of the rats
	for (int i = 0; i < storingRats.size(); i++) {
		if (storingRats[i].getToRemove() == true) {
		}
		//If rat is killed
		else if (storingRats[i].getIsKilled() == true) {
			//Generate a random number between 1 and 2, so there is only a 50% chance of a soul spawning
			int rand1 = rand() % 2;
			//If number = 0, spawn soul where rat died
			if (rand1 == 0) {
				glm::vec3 placeToSpawn = storingRatModels[i]->position();
				placeToSpawn.y = 1.0f;
				spawnSouls(placeToSpawn);
			}
		}
	}

	//If the player has not won yet
	if (hasVictory == false) {
		//Update the rats
		for (int i = 0; i < storingRats.size(); i++) {
			storingRats[i].setPosition(glm::vec3(0.0f, -20.f, 0.0f));
			storingRats[i].on_update(time_step, m_player.getThePosition());

		}
	}

	//If player is strafing
	if (isStrafing == true) {
		//Reduce strafe timer
		strafeBack -= 3.0f * time_step;
		//If strafing timer is less than zero, stop player strafing
		if (strafeBack <= 0.0f) {
			isStrafing = false;
			m_player.set_m_speed(0.0f);
			m_player.setAnimation(2);
		}
	}

} 

void example_layer::on_render()
{
	engine::render_command::clear_color({ 0.2f, 0.3f, 0.3f, 1.0f });
	engine::render_command::clear();

	// Set up  shader. (renders textures and materials)
	const auto mesh_shader = engine::renderer::shaders_library()->get("mesh");
	engine::renderer::begin_scene(m_3d_camera, mesh_shader);

	// Set up some of the scene's parameters in the shader
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

	// Position the skybox centred on the player and render it
	glm::mat4 skybox_tranform(1.0f);
	skybox_tranform = glm::translate(skybox_tranform, m_3d_camera.position());
	for (const auto& texture : m_skybox->textures())
	{
		texture->bind();
	}
	engine::renderer::submit(mesh_shader, m_skybox, skybox_tranform);

	//Render the terrain
	engine::renderer::submit(mesh_shader, m_terrain);

	//Render the player box
	//m_player.getBox().on_render(2.5f, 0.f, 0.f, mesh_shader);

	//If boss has spawned, render the boss box
	//if (bossHasSpawned == true) {
	//	theBoss.getBox().on_render(2.5f, 0.f, 0.f, mesh_shader);
	//}

	//For all the rats, render the rat boxes
	//for (int i = 0; i < storingRats.size(); i++) {
	//	storingRats[i].getBox().on_render(2.5f, 0.0f, 0.0f, mesh_shader);
	//}

	//If the user tries to show the blank balls, render them
	if (blankShow) {
		for (int i = 0; i < storingBlankBalls.size(); i++) {
			engine::renderer::submit(mesh_shader, storingBlankBalls[i]);
		}
	}

	//Render the spell circle
	m_spell_circle.on_render(mesh_shader);

	//Render the fallen warrior model
	engine::renderer::submit(mesh_shader, m_dude);

	//Transforms for the throne
	glm::mat4 throne_transform(1.0f);
	throne_transform = glm::translate(throne_transform, m_throne->position());
	//Set the rotation for the throne
	m_throne->set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));
	throne_transform = glm::rotate(throne_transform, 2.3f, m_throne->rotation_axis());
	m_throne->set_rotation_axis(glm::vec3(1.0f, 0.0f, 0.0f));
	throne_transform = glm::rotate(throne_transform, -1.57f, m_throne->rotation_axis());
	//Scaled to the best size from 3rd person perspective
	throne_transform = glm::scale(throne_transform, m_throne->scale() * 1.2f);
	engine::renderer::submit(mesh_shader, throne_transform, m_throne);

	//If victory hasn't started
	if (hasVictory == false && showFinalBall == false) {
		if (bossHasSpawned == true) {
			//Sets transforms for the boss, all predefined
			glm::mat4 boss_transform(1.0f);
			boss_transform = glm::translate(boss_transform, m_boss->position());
			boss_transform = glm::rotate(boss_transform, m_boss->rotation_amount(), m_boss->rotation_axis());
			boss_transform = glm::translate(boss_transform, -m_boss->offset() * m_boss->scale());
			boss_transform = glm::scale(boss_transform, m_boss->scale());
			engine::renderer::submit(mesh_shader, boss_transform, m_boss);
		}
	}

	//If victory hasn't started
	if (hasVictory == false && showFinalBall == false) {
		//For all the rats
		for (int i = 0; i < storingRatModels.size(); i++) {
			//Sets transforms for the rat, all predefined
			glm::mat4 rat_transform(1.0f);
			rat_transform = glm::translate(rat_transform, storingRatModels[i]->position());
			rat_transform = glm::rotate(rat_transform, storingRatModels[i]->rotation_amount(), storingRatModels[i]->rotation_axis());
			rat_transform = glm::translate(rat_transform, -storingRatModels[i]->offset() * storingRatModels[i]->scale());
			rat_transform = glm::scale(rat_transform, storingRatModels[i]->scale());
			engine::renderer::submit(mesh_shader, rat_transform, storingRatModels[i]);
		}
	}

	//Method to spawn cross meshes circling around boss area. Specific rotations to ensure the crosses always point inwards
	crossSpawn(mesh_shader, glm::vec3(2.2f, 0.5f, 19.4f), 1.57);
	crossSpawn(mesh_shader, glm::vec3(4.5f, 0.5f, 27.2f), 1.57);
	crossSpawn(mesh_shader, glm::vec3(3.f, 0.5f, 37.f), 0.785);
	crossSpawn(mesh_shader, glm::vec3(-5.1f, 0.5f, 39.9f), 3.14);
	crossSpawn(mesh_shader, glm::vec3(-13.4f, 0.5f, 39.8f), 3.14);
	crossSpawn(mesh_shader, glm::vec3(-21.3f, 0.5f, 39.7f), 3.14);
	crossSpawn(mesh_shader, glm::vec3(-31.f, 0.5f, 40.7f), 3.14);
	crossSpawn(mesh_shader, glm::vec3(-40.5f, 0.5f, 39.8f), 2.355);
	crossSpawn(mesh_shader, glm::vec3(-42.f, 0.5f, 31.3f), 1.57);
	crossSpawn(mesh_shader, glm::vec3(-43.f, 0.5f, 23.f), 1.57);
	crossSpawn(mesh_shader, glm::vec3(-43.8f, 0.5f, 10.4f), 1.57);
	crossSpawn(mesh_shader, glm::vec3(-36.9f, 0.5f, 6.3f), 3.14);
	crossSpawn(mesh_shader, glm::vec3(-30.5f, 0.5f, 5.2f), 3.14);
	crossSpawn(mesh_shader, glm::vec3(-15.2f, 0.5f, 2.7f), 3.14);

	//Sets rotation axis for the sword and shield
	m_blade->set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));
	m_hilt->set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));
	m_shield->set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));
	m_trim->set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));

	//Method to spawn swords flat on the ground given a location and rotation
	flatSwordSpawn(mesh_shader, glm::vec3(39.5f, 0.45f, -34.1f), 0.7f);
	flatShieldSpawn(mesh_shader, glm::vec3(38.6f, -5.45f, -32.6f), 1.0f);
	flatSwordSpawn(mesh_shader, glm::vec3(41.0f, 0.45f, -28.4f), 1.4f);
	flatSwordSpawn(mesh_shader, glm::vec3(35.6f, 0.45f, -23.1f), 2.5f);
	flatShieldSpawn(mesh_shader, glm::vec3(34.0f, -5.45f, -21.5f), 3.1f);
	flatSwordSpawn(mesh_shader, glm::vec3(34.8f, 0.45f, -20.0f), 2.0f);
	flatSwordSpawn(mesh_shader, glm::vec3(37.7f, 0.45f, -14.5f), 0.9f);
	flatShieldSpawn(mesh_shader, glm::vec3(38.2f, -5.45f, -11.2f), 1.7f);
	flatSwordSpawn(mesh_shader, glm::vec3(34.5f, 0.45f, -7.8f), 0.5f);
	flatSwordSpawn(mesh_shader, glm::vec3(32.3f, 0.45f, -6.8f), 0.3f);
	flatShieldSpawn(mesh_shader, glm::vec3(29.3f, -5.45f, 1.9f), 1.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(25.0f, 0.45f, 5.5f), 2.9f);
	flatSwordSpawn(mesh_shader, glm::vec3(22.5f, 0.45f, 9.1f), 1.8f);
	flatShieldSpawn(mesh_shader, glm::vec3(23.4f, -5.45f, 12.8f), 2.2f);
	flatSwordSpawn(mesh_shader, glm::vec3(28.8f, 0.45f, 16.9f), 2.7f);
	flatSwordSpawn(mesh_shader, glm::vec3(31.4f, 0.45f, 19.9f), 0.3f);
	flatShieldSpawn(mesh_shader, glm::vec3(31.0f, -5.45f, 23.8f), 3.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(34.0f, 0.45f, 1.4f), 2.5f);
	flatSwordSpawn(mesh_shader, glm::vec3(35.8f, 0.45f, 4.3f), 1.7f);
	flatShieldSpawn(mesh_shader, glm::vec3(36.2f, -5.45f, 6.8f), 1.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(34.8f, 0.45f, 9.5f), 0.2f);
	flatSwordSpawn(mesh_shader, glm::vec3(35.2f, 0.45f, 14.5f), 3.3f);
	flatShieldSpawn(mesh_shader, glm::vec3(31.7f, -5.45f, -26.8f), 1.8f);
	flatSwordSpawn(mesh_shader, glm::vec3(30.5f, 0.45f, -20.9f), 2.2f);
	flatSwordSpawn(mesh_shader, glm::vec3(31.2f, 0.45f, -16.7f), 2.7f);
	flatShieldSpawn(mesh_shader, glm::vec3(30.8f, -5.45f, -11.4f), 0.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(27.4f, 0.45f, -5.4f), 3.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(24.9f, 0.45f, -0.6f), 2.5f);
	flatShieldSpawn(mesh_shader, glm::vec3(20.1f, -5.45f, 3.01f), 1.7f);
	flatSwordSpawn(mesh_shader, glm::vec3(15.8f, 0.45f, 4.43f), 1.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(12.5f, 0.45f, 2.60f), 0.2f);
	flatShieldSpawn(mesh_shader, glm::vec3(11.5f, -5.45f, -1.5f), 3.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(13.1f, 0.45f, -4.7f), 0.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(14.6f, 0.45f, -10.5), 2.5f);
	flatShieldSpawn(mesh_shader, glm::vec3(15.0f, -5.45f, -13.7f), 1.7f);
	flatSwordSpawn(mesh_shader, glm::vec3(14.6f, 0.45f, -18.46), 1.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(16.0f, 0.45f, -22.0f), 0.2f);
	flatShieldSpawn(mesh_shader, glm::vec3(18.7f, -5.45f, -25.5f), 3.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(19.7f, 0.45f, -29.3f), 3.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(17.6f, 0.45f, -33.3f), 2.5f);
	flatShieldSpawn(mesh_shader, glm::vec3(14.5f, -5.45f, -35.0f), 1.7f);
	flatSwordSpawn(mesh_shader, glm::vec3(11.7f, 0.45f, -34.7f), 1.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(9.27f, 0.45f, -32.4f), 0.2f);
	flatShieldSpawn(mesh_shader, glm::vec3(6.70f, -5.45f, -29.4f), 3.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(-8.4f, 0.45f, -33.3f), 1.7f);
	flatSwordSpawn(mesh_shader, glm::vec3(-10.9f, 0.45f, -34.7f), 1.3f);
	flatShieldSpawn(mesh_shader, glm::vec3(-13.8f, -5.45f, -34.6f), 0.2f);
	flatSwordSpawn(mesh_shader, glm::vec3(-16.6f, 0.45f, -32.1f), 3.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(-17.6f, 0.45f, -32.2f), 3.3f);
	flatShieldSpawn(mesh_shader, glm::vec3(-19.1f, -5.45f, -20.9f), 2.5f);
	flatSwordSpawn(mesh_shader, glm::vec3(-22.9f, 0.45f, -15.8f), 1.7f);
	flatSwordSpawn(mesh_shader, glm::vec3(-25.2f, 0.45f, -14.2f), 1.3f);
	flatShieldSpawn(mesh_shader, glm::vec3(-32.1f, -5.45f, -14.3f), 3.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(12.10f, 0.45f, -24.7f), 0.2f);
	flatSwordSpawn(mesh_shader, glm::vec3(10.80f, 0.45f, -26.3f), 3.3f);
	flatShieldSpawn(mesh_shader, glm::vec3(7.20f, -5.45f, -26.5f), 3.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(6.00f, 0.45f, -24.9f), 2.5f);
	flatSwordSpawn(mesh_shader, glm::vec3(4.74f, 0.45f, -19.8f), 1.7f);
	flatShieldSpawn(mesh_shader, glm::vec3(-1.5f, -5.45f, -17.4f), 1.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(-7.5f, 0.45f, -16.7f), 0.2f);
	flatSwordSpawn(mesh_shader, glm::vec3(-12.6f, 0.45f, -16.9f), 3.3f);
	flatShieldSpawn(mesh_shader, glm::vec3(-14.8f, -5.45f, -19.1f), 0.2f);
	flatSwordSpawn(mesh_shader, glm::vec3(-13.8f, 0.45f, -23.2f), 3.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(-8.4f, 0.45f, -27.3f), 3.3f);
	flatShieldSpawn(mesh_shader, glm::vec3(-2.3f, -5.45f, -35.5f), 2.5f);
	flatSwordSpawn(mesh_shader, glm::vec3(-14.2f, 0.45f, -37.2f), 1.7f);
	flatSwordSpawn(mesh_shader, glm::vec3(-19.4f, 0.45f, -36.2f), 1.3f);
	flatShieldSpawn(mesh_shader, glm::vec3(-20.8f, -5.45f, -34.1f), 0.2f);
	flatSwordSpawn(mesh_shader, glm::vec3(-20.6f, 0.45f, -25.8f), 0.2f);
	flatSwordSpawn(mesh_shader, glm::vec3(-22.4f, 0.45f, -21.7f), 3.3f);
	flatShieldSpawn(mesh_shader, glm::vec3(-26.8f, -5.45f, -18.9f), 3.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(-30.4f, 0.45f, -18.5f), 2.5f);
	flatSwordSpawn(mesh_shader, glm::vec3(-33.8f, 0.45f, -21.4f), 1.7f);
	flatShieldSpawn(mesh_shader, glm::vec3(-33.4f, -5.45f, -27.5f), 1.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(-28.9f, 0.45f, -31.0f), 0.2f);
	flatSwordSpawn(mesh_shader, glm::vec3(25.9f, 0.45f, -17.7f), 3.3f);
	flatShieldSpawn(mesh_shader, glm::vec3(25.5f, -5.45f, -10.3f), 3.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(23.4f, 0.45f, -4.60f), 2.5f);
	flatSwordSpawn(mesh_shader, glm::vec3(21.4f, 0.45f, 4.79f), 1.7f);
	flatShieldSpawn(mesh_shader, glm::vec3(17.5f, -5.45f, 8.33f), 1.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(9.40f, 0.45f, -6.7f), 0.2f);
	flatSwordSpawn(mesh_shader, glm::vec3(60.3f, 0.45f, -11.8f), 0.2f);
	flatShieldSpawn(mesh_shader, glm::vec3(3.46f, -5.45f, -14.1f), 3.3f);
	flatSwordSpawn(mesh_shader, glm::vec3(2.70f, 0.45f, -20.1f), 2.5f);
	flatSwordSpawn(mesh_shader, glm::vec3(2.24f, 0.45f, -23.3f), 1.7f);
	flatShieldSpawn(mesh_shader, glm::vec3(-1.8f, -5.45f, -26.0f), 0.2f);
	flatSwordSpawn(mesh_shader, glm::vec3(-1.76f, 0.45f, -23.9f), 1.7f);
	flatSwordSpawn(mesh_shader, glm::vec3(-2.0f, 0.45f, -22.3f), 1.3f);
	flatShieldSpawn(mesh_shader, glm::vec3(-5.7f, -5.45f, -21.2f), 0.2f);
	flatSwordSpawn(mesh_shader, glm::vec3(27.9f, 0.45f, -27.5f), 1.7f);
	flatSwordSpawn(mesh_shader, glm::vec3(23.7f, 0.45f, -23.3f), 1.3f);
	flatShieldSpawn(mesh_shader, glm::vec3(20.7f, -5.45f, -17.7f), 0.2f);
	flatSwordSpawn(mesh_shader, glm::vec3(19.6f, 0.45f, -10.8f), 1.7f);
	flatSwordSpawn(mesh_shader, glm::vec3(18.8f, 0.45f, -4.44f), 1.3f);
	flatShieldSpawn(mesh_shader, glm::vec3(16.9f, -5.45f, 0.48f), 0.2f);

	//Methods to spawn trio's of swords stuck into the ground, all positioned and rotated to look as if stabbed into the ground
	swordSpawn(mesh_shader, glm::vec3(37.0f, 1.0f, -36.0f), 1.35, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
	swordSpawn(mesh_shader, glm::vec3(37.0f, 1.0f, -36.0f), 1.75, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.9f);
	swordSpawn(mesh_shader, glm::vec3(37.0f, 1.0f, -35.6f), 1.80, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), -0.4f);

	swordSpawn(mesh_shader, glm::vec3(28.2f, 1.0f, 26.2f), 1.35, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
	swordSpawn(mesh_shader, glm::vec3(28.2f, 1.0f, 26.2f), 1.75, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.9f);
	swordSpawn(mesh_shader, glm::vec3(28.2f, 1.0f, 25.8f), 1.80, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), -0.4f);

	swordSpawn(mesh_shader, glm::vec3(30.0f, 1.0f, -2.5f), 1.35, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
	swordSpawn(mesh_shader, glm::vec3(30.0f, 1.0f, -2.5f), 1.75, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.9f);
	swordSpawn(mesh_shader, glm::vec3(30.0f, 1.0f, -2.9f), 1.80, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), -0.4f);

	swordSpawn(mesh_shader, glm::vec3(36.9f, 1.0f, -25.4f), 1.35, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
	swordSpawn(mesh_shader, glm::vec3(36.9f, 1.0f, -25.4f), 1.75, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.9f);
	swordSpawn(mesh_shader, glm::vec3(36.9f, 1.0f, -25.8f), 1.80, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), -0.4f);

	swordSpawn(mesh_shader, glm::vec3(3.40f, 1.0f, -17.9f), 1.35, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
	swordSpawn(mesh_shader, glm::vec3(3.40f, 1.0f, -17.9f), 1.75, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.9f);
	swordSpawn(mesh_shader, glm::vec3(3.40f, 1.0f, -17.5f), 1.80, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), -0.4f);

	swordSpawn(mesh_shader, glm::vec3(0.50f, 1.0f, -25.2f), 1.35, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
	swordSpawn(mesh_shader, glm::vec3(0.50f, 1.0f, -25.2f), 1.75, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.9f);
	swordSpawn(mesh_shader, glm::vec3(0.50f, 1.0f, -25.6f), 1.80, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), -0.4f);

	swordSpawn(mesh_shader, glm::vec3(26.1f, 1.0f, -21.5f), 1.35, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
	swordSpawn(mesh_shader, glm::vec3(26.1f, 1.0f, -21.5f), 1.75, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.9f);
	swordSpawn(mesh_shader, glm::vec3(26.1f, 1.0f, -21.9f), 1.80, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), -0.4f);

	swordSpawn(mesh_shader, glm::vec3(-21.1f, 1.0f, -31.6f), 1.35, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
	swordSpawn(mesh_shader, glm::vec3(-21.1f, 1.0f, -31.6f), 1.75, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.9f);
	swordSpawn(mesh_shader, glm::vec3(-21.1f, 1.0f, -32.0f), 1.80, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), -0.4f);

	swordSpawn(mesh_shader, glm::vec3(-27.9f, 1.0f, -14.2f), 1.35, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
	swordSpawn(mesh_shader, glm::vec3(-27.9f, 1.0f, -14.2f), 1.75, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.9f);
	swordSpawn(mesh_shader, glm::vec3(-27.9f, 1.0f, -14.6f), 1.80, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), -0.4f);

	//Rendering the swords
	engine::renderer::submit(mesh_shader, m_blade);
	engine::renderer::submit(mesh_shader, m_hilt);

	//Rendering the shields
	engine::renderer::submit(mesh_shader, m_shield);
	engine::renderer::submit(mesh_shader, m_trim);

	//Renders m_plasma
	m_plasma->on_render(mesh_shader);

	//Renders the flame effect
	theFlame.on_render(m_3d_camera, mesh_shader);

	//Render the plasma
	for (int i = 0; i < storingPlasma.size(); i++) {
		storingPlasma[i]->on_render(mesh_shader);
	}

	//Render the plasma balls
	//for (int i = 0; i < storingPlasmaBalls.size(); i++) {
	//	engine::renderer::submit(mesh_shader, storingPlasmaBalls[i]);
	//}

	//Render the boss plasma
	for (int i = 0; i < storingBossPlasma.size(); i++) {
		storingBossPlasma[i]->on_render(mesh_shader);
	}

	//Render the boss plasma balls
	//for (int i = 0; i < storingBossPlasmaBalls.size(); i++) {
	//	engine::renderer::submit(mesh_shader, storingBossPlasmaBalls[i]);
	//}

	//Submits m_material
	m_material->submit(mesh_shader);

	//Renders and sets material for the mannequin
	m_mannequin_material->submit(mesh_shader);
	engine::renderer::submit(mesh_shader, m_mannequin);

	//Health pack rendering
	for (int i = 0; i < storingHealth.size(); i++) {
		if (storingHealth[i]->m_is_active == true) {
			std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", true);
			storingHealth[i]->textures().at(0)->bind();
			glm::mat4 health_transform(1.0f);
			health_transform = glm::translate(health_transform, storingHealth[i]->position());
			storingHealth[i]->set_rotation_axis(glm::vec3(0.f, 1.f, 1.f));

			health_transform = glm::rotate(health_transform, storingHealth[i]->rotation_amount(), storingHealth[i]->rotation_axis());
			engine::renderer::submit(mesh_shader, storingHealth[i]->meshes().at(0), health_transform);
			std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", false);
		}
	}

	//Boss health pack rendering
	for (int i = 0; i < storingBossHealth.size(); i++) {
		if (storingBossHealth[i]->m_is_active == true) {
			std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", true);
			storingBossHealth[i]->textures().at(0)->bind();
			glm::mat4 health_transform(1.0f);
			health_transform = glm::translate(health_transform, storingBossHealth[i]->position());
			storingBossHealth[i]->set_rotation_axis(glm::vec3(0.f, 1.f, 1.f));

			health_transform = glm::rotate(health_transform, storingBossHealth[i]->rotation_amount(), storingBossHealth[i]->rotation_axis());
			engine::renderer::submit(mesh_shader, storingBossHealth[i]->meshes().at(0), health_transform);
			std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", false);
		}
	}

	//Soul rendering
	for (int i = 0; i < storingSouls.size(); i++) {
		if (storingSouls[i]->m_is_active == true) {
			std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", true);
			storingSouls[i]->textures().at(0)->bind();
			glm::mat4 soul_transform(1.0f);
			soul_transform = glm::translate(soul_transform, storingSouls[i]->position());
			storingSouls[i]->set_rotation_axis(glm::vec3(0.f, 1.f, 0.f));

			soul_transform = glm::rotate(soul_transform, storingSouls[i]->rotation_amount(), storingSouls[i]->rotation_axis());
			engine::renderer::submit(mesh_shader, storingSouls[i]->meshes().at(0), soul_transform);
			std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", false);
		}
	}

	//Submits material for player object
	m_mannequin_material->submit(mesh_shader);
	engine::renderer::submit(mesh_shader, m_player.object());

	engine::renderer::end_scene();

	// Render text
	const auto text_shader = engine::renderer::shaders_library()->get("text_2D");

	//If the map is closed, render the current quest and timer
	//All locations specific to quest to ensure they are always centered
	if (getIsMap() == false) {
		if (questNumber == 0) {
			m_text_manager->render_text(text_shader, info, 600.f, (float)engine::application::window().height() - 50.f, 1.0f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
		}
		else if (questNumber == 1) {
			m_text_manager->render_text(text_shader, info, 850.f, (float)engine::application::window().height() - 50.f, 1.0f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
		}
		else if (questNumber == 2) {
			m_text_manager->render_text(text_shader, info, 900.f, (float)engine::application::window().height() - 50.f, 1.0f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
		}
		else if (questNumber == 3) {
			m_text_manager->render_text(text_shader, info, 930.f, (float)engine::application::window().height() - 50.f, 1.0f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
		}
		m_text_manager->render_text(text_shader, timer, 2000.f, (float)engine::application::window().height() - 380.f, 2.3f, glm::vec4(0.376f, 0.082f, 0.741f, 1.f));
	}

	engine::renderer::begin_scene(m_2d_camera, mesh_shader);

	//Render the shaders for the info screens
	m_intro->on_render(mesh_shader);
	m_controls->on_render(mesh_shader);
	m_quest->on_render(mesh_shader);
	m_on_fire->on_render(mesh_shader);
	m_died->on_render(mesh_shader);
	m_lose->on_render(mesh_shader);

	//Renders the soul holder HUD elements
	m_soulHolder0->on_render(mesh_shader);
	m_soulHolder1->on_render(mesh_shader);
	m_soulHolder2->on_render(mesh_shader);
	m_soulHolder3->on_render(mesh_shader);
	m_soulHolder4->on_render(mesh_shader);
	m_soulHolder5->on_render(mesh_shader);
	m_soulHolder6->on_render(mesh_shader);
	m_soulHolder7->on_render(mesh_shader);

	//Renders the health bar HUD elements
	m_healthBar0->on_render(mesh_shader);
	m_healthBar5->on_render(mesh_shader);
	m_healthBar10->on_render(mesh_shader);
	m_healthBar15->on_render(mesh_shader);
	m_healthBar20->on_render(mesh_shader);
	m_healthBar25->on_render(mesh_shader);
	m_healthBar30->on_render(mesh_shader);
	m_healthBar35->on_render(mesh_shader);
	m_healthBar40->on_render(mesh_shader);
	m_healthBar45->on_render(mesh_shader);
	m_healthBar50->on_render(mesh_shader);
	m_healthBar55->on_render(mesh_shader);
	m_healthBar60->on_render(mesh_shader);
	m_healthBar65->on_render(mesh_shader);
	m_healthBar70->on_render(mesh_shader);
	m_healthBar75->on_render(mesh_shader);
	m_healthBar80->on_render(mesh_shader);
	m_healthBar85->on_render(mesh_shader);
	m_healthBar90->on_render(mesh_shader);
	m_healthBar95->on_render(mesh_shader);
	m_healthBar100->on_render(mesh_shader);

	//Sets m_material shader
	m_material->submit(mesh_shader);

	//Sets the rotation axis for the man model
	m_man->set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));

	//For the amount of active lights, submit to the shader
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gNumPointLights", (int)num_point_lights);
	for (int i = 0; i < storingPlasmaLights.size(); i++) {
		storingPlasmaLights[i]->submit(mesh_shader, i);
	}


	//Spotlight on spell circle
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gNumSpotLights", (int)num_spot_lights);
	m_spotLight.submit(mesh_shader, 0);
	engine::renderer::submit(mesh_shader, m_ball->meshes().at(0), glm::translate(glm::mat4(1.f), m_spotLight.Position));

	//Final ball light
	if (showFinalBall == true) {
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gNumPointLights", (int)num_final_lights);
		m_finalLight.submit(mesh_shader, 0);
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("lighting_on", false);
		m_lightsource_material->submit(mesh_shader);
		engine::renderer::submit(mesh_shader, m_ball->meshes().at(0), glm::translate(glm::mat4(1.f), m_finalLight.Position));
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("lighting_on", true);
		//If the final ball method is active
		if(showFinalBall == true){
			//Grows each ball
			for (int i = 0; i < ballNumber; i++) {
				float toMultiply = ballMultiply;
				if (i == 2) {
					toMultiply = toMultiply - 20;
				}
				if (i == 3) {
					toMultiply = toMultiply - 40;
				}
				if (i == 4) {
					toMultiply = toMultiply - 60;
				}
				if (i == 5) {
					toMultiply = toMultiply - 80;
				}
				if (i == 6) {
					toMultiply = toMultiply - 100;
				}
				if (i == 7) {
					toMultiply = toMultiply - 120;
				}
				//Transform for the ball
				glm::mat4 ball_transform(1.0f);
				ball_transform = glm::scale(ball_transform, m_ball->scale() * toMultiply);
				engine::renderer::submit(mesh_shader, ball_transform, m_ball);
			}
		}
	}

	//If the man is shown (i.e. the control screen is open)
	if (m_man->getActive() == true) {
		//Render the shader for the man model
		engine::renderer::submit(mesh_shader, m_man);
	}

	engine::renderer::end_scene();
} 

//Event manager
void example_layer::on_event(engine::event& event)
{
	if (event.event_type() == engine::event_type_e::key_pressed)
	{
		auto& e = dynamic_cast<engine::key_pressed_event&>(event);
		if (e.key_code() == engine::key_codes::KEY_TAB)
		{
			engine::render_command::toggle_wireframe();
		}
		//The 'M' key opens and closes the map
		if (e.key_code() == engine::key_codes::KEY_M)
		{
			if (isMap == false) {
				isMap = true;
			}
			else {
				isMap = false;
			}
		}
		//Used to close intro, control and quest screen
		if (e.key_code() == engine::key_codes::KEY_SPACE)
		{
			if (m_player.getDeath() == false) {
				//If controls screen is open
				if (getIsControls() == true) {
					//If this is the start up screen
					if (isInitial == true) {
						//Record state
						setIsControls(false);
						setIsIntro(true);
						//Close control screen
						m_controls->deactivate();
						//Remove man model
						m_man->deactivate();
						//Show intro screen
						m_intro->activate();
						isInitial = false;
					}
					else {
						//Record state
						setIsControls(false);
						//Close control screen
						m_controls->deactivate();
						//Remove man model
						m_man->deactivate();
						//Show current quest
						hideQuest = false;
					}
				}
				//If intro screen is open
				else if (getIsIntro() == true) {
					//Record state
					setIsIntro(false);
					//Deactivate intro screen
					//m_intro->deactivate();
					//Show current quest
					hideQuest = false;
					//Begins fade out
					m_intro->startTheFade();
				}
				//If quest screen is open
				else if (getIsQuest() == true) {
					//Record state
					setIsQuest(false);
					//Close quest screen
					m_quest->deactivate();
					//Show current quest
					hideQuest = false;
					startMainTimer = true;
				}
				else {
					//If not holding plasma
					if (isHoldingPlasma == false) {
						//Spawn plasma in front of player
						spawnPlasma(glm::vec3(m_player.getThePosition().x + (m_player.getDirection().x / 2), 1.0f, m_player.getThePosition().z + (m_player.getDirection().z / 2)));
						//Record that plasma is being held
						isHoldingPlasma = true;
					}
				}
			}
		}
		//Used to open and close the controls screen when the user presses c
		if (e.key_code() == engine::key_codes::KEY_C)
		{
			if (m_player.getDeath() == false) {
				//If control screen is closed
				if (getIsControls() == false) {
					showMainTimer = false;
					hideQuest = true;
					//Open controls screen and spawn man model
					setIsControls(true);
					m_controls->activate();
					m_man->activate();
				}
				//If controls screen is open
				else if (getIsControls() == true) {
					showMainTimer = true;
					hideQuest = false;
					//Close control screen and remove man model
					setIsControls(false);
					m_controls->deactivate();
					m_man->deactivate();
				}
			}
		}
		//Used for shift controls if the user holds either shift key
		if (e.key_code() == engine::key_codes::KEY_LEFT_SHIFT || e.key_code() == engine::key_codes::KEY_RIGHT_SHIFT)
		{
			if (m_player.getIsRunning() == false && m_player.getIsWalk() == true){
				//Sets player to running, with a greater speed and a running animation
				m_player.set_m_speed(5.0f);
				m_player.setRunning(true);
				m_player.setAnimation(4);
				m_player.setRunning(true);
			}
		}
		if (engine::input::key_pressed(engine::key_codes::KEY_W)) {
			//If player is alive
			if (m_player.getDeath() == false) {
				//If the player is not running, set the speed and animation to walk
				if (m_player.getIsRunning() == false) {
					if (m_player.getIsWalk() == false) {
						m_player.setAnimation(1);
						m_player.set_m_speed(2.f);
						m_player.setWalk(true);
					}
				}
			}
		}
		if (engine::input::key_pressed(engine::key_codes::KEY_S)) {
			//If player is alive and not already strafing
			if (m_player.getDeath() == false && isStrafing == false) {
				//Set speed to backwards and start strafe timer
				m_player.set_m_speed(-8.0f);
				strafeBack = 1.0f;
				isStrafing = true;
				m_player.setAnimation(4);
			}
		}
		if (engine::input::key_pressed(engine::key_codes::KEY_J)) {
			soulAmount += 1;
		}
		if (engine::input::key_pressed(engine::key_codes::KEY_K)) {
			showFinalBall = true;
		}
		if (engine::input::key_pressed(engine::key_codes::KEY_L)) {
			defeat();
		}
		if (engine::input::key_pressed(engine::key_codes::KEY_P)) {
			m_player.increaseHealth(20);
		}
	}
	//Key release methods
	if (event.event_type() == engine::event_type_e::key_released)
	{
		auto& e = dynamic_cast<engine::key_released_event&>(event);
		//If player releases shift, slow player to a walk
		if (e.key_code() == engine::key_codes::KEY_LEFT_SHIFT || e.key_code() == engine::key_codes::KEY_RIGHT_SHIFT)
		{
			if (m_player.getDeath() == false) {
				if (m_player.getIsRunning() == true) {
					//Slow player speed and set animation to walk. Record these states
					m_player.set_m_speed(2.0f);
					m_player.setRunning(false);
					m_player.setAnimation(1);
					m_player.setWalk(true);
					m_player.setRunning(false);
				}
			}
		}
		//If player releases W, stop movement
		if (e.key_code() == engine::key_codes::KEY_W)
		{
			if (m_player.getDeath() == false) {
				//Set speed to zero
				m_player.set_m_speed(0.0f);
				//Change animation to still
				m_player.setAnimation(2);
				//Record this state
				m_player.setWalk(false);
				m_player.setRunning(false);
			}
		}
		if (e.key_code() == engine::key_codes::KEY_SPACE)
		{
			//If less than 8 lights exist
				if (m_player.getDeath() == false && num_point_lights <= 8) {
					//Release the plasma that is held
					for (int i = 0; i < heldPlasma.size(); i++) {
						heldPlasma[i]->release();
					}
					//Empty heldPlasma vector
					heldPlasma.clear();
					isHoldingPlasma = false;
					//Creates a new plasma light at plasma position
					createPlasmaLight((glm::vec3(m_player.getThePosition().x, 1.1f, m_player.getThePosition().z)));
				}
				//If plasma lights is 8 or more
				else if (m_player.getDeath() == false && num_point_lights >= 8) {
					//Release the plasma that is held
					for (int i = 0; i < heldPlasma.size(); i++) {
						heldPlasma[i]->release();
					}
					//Empty heldPlasma vector
					heldPlasma.clear();
					isHoldingPlasma = false;

					//For all the plasma lights that are available to use
					for (int i = 0; i <= availablePlasma.size(); i++) {
						//If the light is availble to use
						if (availablePlasma[i] == true) {
							//Set position to new plasma
							storingPlasmaLights[i]->Position = (glm::vec3(m_player.getThePosition().x, 1.1f, m_player.getThePosition().z));
							lastPlasmaLightPosition[i] = (glm::vec3(m_player.getThePosition().x, 1.1f, m_player.getThePosition().z));
							//Sets light colour to purple
							storingPlasmaLights[i]->Color = (glm::vec3(0.5f, 0.0f, 1.0f));
							//Sets time remaining so the light lasts for 2 seconds
							plasmaTimeRemaining[i] = 2.0f;
							//Sets the available plasma to false
							availablePlasma[i] = false;
							break;
						}
					}
				}
				//If number of plasma balls is less than 10
				if (numberOfPlasmaBalls <= 10) {
					//Spawn a plasma ball
					spawnPlasmaBall(glm::vec3(m_player.getThePosition().x + (m_player.getDirection().x / 2), 1.0f, m_player.getThePosition().z + (m_player.getDirection().z / 2)));
				}
				else {
					for (int i = 0; i <= availablePlasmaBall.size(); i++) {
						//If plasma ball is available
						if (availablePlasmaBall[i] == true) {
							//Sets plasma ball position to the new plasma location
							storingPlasmaBalls[i]->set_position(glm::vec3(m_player.getThePosition().x + (m_player.getDirection().x / 2), 1.1f, m_player.getThePosition().z + (m_player.getDirection().z / 2)));
							lastPlasmaBallPosition[i] = (glm::vec3(m_player.getThePosition().x + (m_player.getDirection().x / 2), 1.1f, m_player.getThePosition().z));
							//Sets plasma ball time remaining
							plasmaBallTimeRemaining[i] = 1.9f;
							//Sets available plasma to false
							availablePlasmaBall[i] = false;
							//Sets that plasma can cause collisions
							plasmaCanAffect[i] = true;
							break;
						}
					}
				}
				//Play zap sound
				m_audio_manager->play("zap");
		}
	}
}
//Function to check which camera needs to be used
void example_layer::minimap() {
		if (getIsMap() == false) {
			//Uses the 3rd person camera
			m_player.update_camera(m_3d_camera);
		}
		else if (getIsMap() == true) {
			//Uses the map camera
			m_player.newCam(m_3d_camera);
		}
}

//Method to spawn crosses at specific locations and with specific rotations
void example_layer::crossSpawn(const::engine::ref<engine::shader> m, glm::vec3 v, float f) {
	glm::mat4 cross_transform(1.0f);
	//Translates the cross to given coordinates
	cross_transform = glm::translate(cross_transform, v);
	//Rotates crosses to given rotations
	cross_transform = glm::rotate(cross_transform, f, m_cross->rotation_axis());
	//Scales cross to specific size
	cross_transform = glm::scale(cross_transform, m_cross->scale() * 4.0f);
	engine::renderer::submit(m, cross_transform, m_cross);
}

//Method to spawn the trio's of swords
void example_layer::swordSpawn(const::engine::ref<engine::shader> m, glm::vec3 position, float rotateAmount1, glm::vec3 rotationAxis1, glm::vec3 rotationAxis2, float rotationAmount2) {
	//Given a position, translate each blade to different rotations and positions to render a trio that look like they have been stabbed into the ground
	glm::mat4 blade_transform(1.0f);
	blade_transform = glm::translate(blade_transform, position);
	blade_transform = glm::scale(blade_transform, m_blade->scale() * 0.4f);
	m_blade->set_rotation_axis(rotationAxis1);
	blade_transform = glm::rotate(blade_transform, rotateAmount1, m_blade->rotation_axis());
	m_blade->set_rotation_axis(rotationAxis2);
	blade_transform = glm::rotate(blade_transform, rotationAmount2, m_blade->rotation_axis());
	engine::renderer::submit(m, blade_transform, m_blade);

	//Repeat for hilt's so they line up
	glm::mat4 hilt_transform(1.0f);
	hilt_transform = glm::translate(hilt_transform, position);
	hilt_transform = glm::scale(hilt_transform, m_hilt->scale() * 0.4f);
	m_hilt->set_rotation_axis(rotationAxis1);
	hilt_transform = glm::rotate(hilt_transform, rotateAmount1, m_hilt->rotation_axis());
	m_hilt->set_rotation_axis(rotationAxis2);
	hilt_transform = glm::rotate(hilt_transform, rotationAmount2, m_blade->rotation_axis());
	engine::renderer::submit(m, hilt_transform, m_hilt);
}

//Method to spawn swords on the ground at specific locations and rotations
void example_layer::flatSwordSpawn(const::engine::ref<engine::shader> m, glm::vec3 position, float f) {
	glm::mat4 blade_transform(1.0f);
	blade_transform = glm::translate(blade_transform, position);
	blade_transform = glm::scale(blade_transform, m_blade->scale() * 0.4f);
	blade_transform = glm::rotate(blade_transform, f, m_blade->rotation_axis());
	engine::renderer::submit(m, blade_transform, m_blade);

	glm::mat4 hilt_transform(1.0f);
	hilt_transform = glm::translate(hilt_transform, position);
	hilt_transform = glm::scale(hilt_transform, m_hilt->scale() * 0.4f);
	hilt_transform = glm::rotate(hilt_transform, f, m_hilt->rotation_axis());
	engine::renderer::submit(m, hilt_transform, m_hilt);
}

//Method to spawn shields on the ground at specific locations and rotations
void example_layer::flatShieldSpawn(const::engine::ref<engine::shader> m, glm::vec3 position, float f) {
	glm::mat4 shield_transform(1.0f);
	shield_transform = glm::translate(shield_transform, position);
	shield_transform = glm::rotate(shield_transform, f, m_shield->rotation_axis());
	shield_transform = glm::scale(shield_transform, m_shield->scale() * 4.0f);
	engine::renderer::submit(m, shield_transform, m_shield);

	glm::mat4 trim_transform(1.0f);
	trim_transform = glm::translate(trim_transform, position);
	trim_transform = glm::rotate(trim_transform, f, m_trim->rotation_axis());
	trim_transform = glm::scale(trim_transform, m_trim->scale() * 4.0f);
	engine::renderer::submit(m, trim_transform, m_trim);
}

//Sets the current health bar state on the HUD
void example_layer::setHealthBarState() {
	//Deactivates all health bars
	m_healthBar0->deactivate();
	m_healthBar5->deactivate();
	m_healthBar10->deactivate();
	m_healthBar15->deactivate();
	m_healthBar20->deactivate();
	m_healthBar25->deactivate();
	m_healthBar30->deactivate();
	m_healthBar35->deactivate();
	m_healthBar40->deactivate();
	m_healthBar45->deactivate();
	m_healthBar50->deactivate();
	m_healthBar55->deactivate();
	m_healthBar60->deactivate();
	m_healthBar65->deactivate();
	m_healthBar70->deactivate();
	m_healthBar75->deactivate();
	m_healthBar80->deactivate();
	m_healthBar85->deactivate();
	m_healthBar90->deactivate();
	m_healthBar95->deactivate();
	m_healthBar100->deactivate();

	//Activates the health bar image based on the current health
	if (m_player.getHealth() >= 100) {
		m_healthBar100->activate();
	}
	else if (m_player.getHealth() >= 95) {
		m_healthBar95->activate();
	}
	else if (m_player.getHealth() >= 90) {
		m_healthBar90->activate();
	}
	else if (m_player.getHealth() >= 85) {
		m_healthBar85->activate();
	}
	else if (m_player.getHealth() >= 80) {
		m_healthBar80->activate();
	}
	else if (m_player.getHealth() >= 75) {
		m_healthBar75->activate();
	}
	else if (m_player.getHealth() >= 70) {
		m_healthBar70->activate();
	}
	else if (m_player.getHealth() >= 65) {
		m_healthBar65->activate();
	}
	else if (m_player.getHealth() >= 60) {
		m_healthBar60->activate();
	}
	else if (m_player.getHealth() >= 55) {
		m_healthBar55->activate();
	}
	else if (m_player.getHealth() >= 50) {
		m_healthBar50->activate();
	}
	else if (m_player.getHealth() >= 45) {
		m_healthBar45->activate();
	}
	else if (m_player.getHealth() >= 40) {
		m_healthBar40->activate();
	}
	else if (m_player.getHealth() >= 35) {
		m_healthBar35->activate();
	}
	else if (m_player.getHealth() >= 30) {
		m_healthBar30->activate();
	}
	else if (m_player.getHealth() >= 25) {
		m_healthBar25->activate();
	}
	else if (m_player.getHealth() >= 20) {
		m_healthBar20->activate();
	}
	else if (m_player.getHealth() >= 15) {
		m_healthBar15->activate();
	}
	else if (m_player.getHealth() >= 10) {
		m_healthBar10->activate();
	}
	else if (m_player.getHealth() >= 5) {
		m_healthBar5->activate();
	}
	else if (m_player.getHealth() < 5) {
		m_healthBar0->activate();
	}
}

//Method to run the main timer
void example_layer::theTimer(const engine::timestep& time_step) {
	//If the main game is still being played
	if (hasVictory == false && showFinalBall == false) {
		//Decrease counter value based on the miliseconds that have taken place since the last frame
		counter -= time_step.milliseconds();

		//Convert counter to seconds and minutes
		int seconds = ((int)counter) / 1000;
		int addSeconds = seconds % 60;
		int minutes = (seconds - addSeconds) / 60;

		//Sets the timer value to the screen
		if (showMainTimer == true) {
			if (addSeconds < 10 && addSeconds >= 0) {
				timer = std::to_string(minutes) + ":0" + std::to_string(addSeconds);
			}
			else {
				timer = std::to_string(minutes) + ":" + std::to_string(addSeconds);
			}
		}
		else {
			timer = "";
		}

		//If the timer runs out, run the defeat method
		if (seconds <= 0) {
			defeat();
		}
	}
}
//Updates each health bar state to keep the necessary one on the screen
void example_layer::updateHealthBars() {
	m_healthBar0->on_update();
	m_healthBar5->on_update();
	m_healthBar10->on_update();
	m_healthBar15->on_update();
	m_healthBar20->on_update();
	m_healthBar25->on_update();
	m_healthBar30->on_update();
	m_healthBar35->on_update();
	m_healthBar40->on_update();
	m_healthBar45->on_update();
	m_healthBar50->on_update();
	m_healthBar55->on_update();
	m_healthBar60->on_update();
	m_healthBar75->on_update();
	m_healthBar80->on_update();
	m_healthBar85->on_update();
	m_healthBar90->on_update();
	m_healthBar95->on_update();
	m_healthBar100->on_update();
}

void example_layer::spawnHealthPack() {

	//Creates position variable with y set to 1
	glm::vec3 thePosition = glm::vec3(0.f, 1.f, 0.f);
	//Generates random int to decide whether to plot point in section 0 or 1
	int zone = rand() % 2;
	//int zone = srand(time(0)) % 2;
	//If in section 0
	if (zone == 0) {
		//Generate random float between x coordinates
		int rand1 = rand() % (3690 - 2080);
		float generateX = (float)rand1;
		generateX = (generateX / 100) + 20.8;
		//Generate random float between z coordinates
		int rand2 = rand() % (2900 + 4700);
		float generateZ = (float)rand2;
		generateZ = (generateZ / 100) - 40.7;
		//Saves the new coordinates
		thePosition.x = generateX;
		thePosition.z = generateZ;
	}
	//If in section 1
	else {
		//Generate random float between x coordinates
		int rand1 = rand() % (2460);
		float generateZ = (float)rand1;
		generateZ = (generateZ / 100) - 37.2;
		//Generate random float between z coordinates
		int rand2 = rand() % (3690 + 3940);
		float generateX = (float)rand2;
		generateX = (generateX / 100) - 39.4;
		//Saves the new coordinates
		thePosition.x = generateX;
		thePosition.z = generateZ;
	}

	//Creates a new health pack at random position
	engine::ref<engine::cuboid> health_shape = engine::cuboid::create(glm::vec3(0.15f), false);
	engine::ref<engine::texture_2d> health_texture = engine::texture_2d::create("assets/textures/health.png", true);
	engine::game_object_properties health_props;
	health_props.position = { thePosition};

	health_props.meshes = { health_shape->mesh() };
	health_props.textures = { health_texture };
	std::shared_ptr<health> current = std::make_shared<health>(health_props);
	current->set_rotation_amount(0.f);

	//Store health pack to vector
	storingHealth.push_back(current);
}

void example_layer::spawnBossHealthPack() {
	//Creates position variable with y set to 1
	glm::vec3 thePosition = glm::vec3(0.f, 1.f, 0.f);
	//If in section 0
	//Generate random float between x coordinates
	int rand1 = rand() % (3843);
	float generateX = (float)rand1;
	generateX = (generateX / 100) -39.4;
	//Generate random float between z coordinates
	int rand2 = rand() % (3151);
	float generateZ = (float)rand2;
	generateZ = (generateZ / 100) + 5.59;
	//Saves the new coordinates
	thePosition.x = generateX;
	thePosition.z = generateZ;

	//Creates a new health pack at random position
	engine::ref<engine::cuboid> health_shape = engine::cuboid::create(glm::vec3(0.15f), false);
	engine::ref<engine::texture_2d> health_texture = engine::texture_2d::create("assets/textures/health.png", true);
	engine::game_object_properties health_props;
	health_props.position = { thePosition };

	health_props.meshes = { health_shape->mesh() };
	health_props.textures = { health_texture };
	std::shared_ptr<health> current = std::make_shared<health>(health_props);
	current->set_rotation_amount(0.f);

	//Store boss health pack to vector
	storingBossHealth.push_back(current);
}

//Method to spawn souls at specific locations
void example_layer::spawnSouls(glm::vec3 p) {
	engine::ref<engine::diamond> diamond_shape = engine::diamond::create(diamond_vertices);
	std::vector<engine::ref<engine::texture_2d>> diamond_textures = { engine::texture_2d::create("assets/textures/soul.png", false) };
	engine::game_object_properties diamond_props;
	diamond_props.position = { p };
	diamond_props.meshes = { diamond_shape->mesh() };
	diamond_props.textures = diamond_textures;
	std::shared_ptr<soul> current = std::make_shared<soul>(diamond_props);

	//Record the souls to vector
	storingSouls.push_back(current);

	//Sets timer for specific soul to remain for 1.5 seconds
	soulTimer.push_back(1.5f);

}
//Starts the player on fire effect
void example_layer::setOnFire() {
	//Activates the fading in and out fire screen
	m_on_fire->activate();
	//Reduces health by 5
	m_player.reduceHealth(5);
	//Starts the timer to reduce health over the effect
	fireTrigger = true;
	//Starts timer max time
	fireCounting = 3.3f;
	//Sets the number of times for the timer to act
	fireCounter = 2;
	whileOnFire = true;
}

//Method to reduce health over time as part of fire effect
void example_layer::fireHit(const engine::timestep& time_step) {
	//If no more timers to run, stop allowing method to run
	if (fireCounter == 0) {
		fireTrigger = false;
		whileOnFire = false;
	}
	//If counter runs out of time
	else if (fireCounting <= 0.0f) {
		//Reset counter to start timer again
		fireCounting = 3.3f;
		//Reduce health by 5
		m_player.reduceHealth(5);
		//Reduce remaining counter runs by 1
		fireCounter -= 1;
	}
	//If no effect needed, reduce time left on timer taking into account frame rate
	fireCounting -= 1.0f * (float)time_step;

}

//Method to reduce time for each diamond
void example_layer::reduceDiamondTimes(const engine::timestep& time_step) {
	std::vector<int> toRemove;
	//For all of the soul timers
	for (int i = 0; i < soulTimer.size(); i++) {
		//If soul timer is less than 0
		if (soulTimer[i] <= 0.0f) {
			//Remove the soul and its records
			storingSouls[i]->remove();
			storingSouls.erase(storingSouls.begin() + i);
			soulTimer.erase(soulTimer.begin() + i);
		}
		else {
			//Decrease soul timer based on time_step
			soulTimer[i] -= 1.0f * (float)time_step;
		}
	}
}
//Method for victory animations
void example_layer::victory() {
	//Creates a new skybox with a blue sky
	m_skybox = engine::skybox::create(100.f,
		{ engine::texture_2d::create("assets/textures/skybox/SkyboxFront.jpg", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxRight.jpg", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxBack.jpg", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxLeft.jpg", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxTop.jpg", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxBottom.jpg", true)
		});

	//Removes the flames from the game
	theFlame.clear();

	//Removes the spell circle from the game
	m_spell_circle.disappear();

	//Removes the spotlight that was on the spell circle
	m_spotLight.AmbientIntensity = 0.0f;
	m_spotLight.DiffuseIntensity = 0.0f;

	//Hides the main timer
	showMainTimer = false;

	//Records the victory method has begun
	hasVictory = true;

	//Pause background music
	m_audio_manager->pause("music");
	//Play summer audio
	m_audio_manager->play("summer");

} 
//Method for defeat animation
void example_layer::defeat() {
	//Hides the main timer
	showMainTimer = false;
	//Starts the spell circle exxpansion effect
	m_spell_circle.expand();
	theBoss.setHealth(1000);
	hasDefeat = true;
}
//Method to grow the lit balls on victory effect to ensure the effect isnt broken
void example_layer::growBall(const engine::timestep& time_step) {
	//Increase the size of the ball over time regardless of processor speed
	ballMultiply += (20.0f * time_step);
	//For each ball, when it reaches a specific size, add another ball
	if (ballNumber == 1) {
		if (ballMultiply >= 20) {
			ballNumber++;
		}
	}
	else if (ballNumber == 2) {
		if (ballMultiply >= 40) {
			ballNumber++;
		}
	}
	else if (ballNumber == 3) {
		if (ballMultiply >= 60) {
			ballNumber++;
		}
	}
	else if (ballNumber == 4) {
		if (ballMultiply >= 80) {
			ballNumber++;
		}
	}
	else if (ballNumber == 5) {
		if (ballMultiply >= 100) {
			ballNumber++;
		}
	}
	else if (ballNumber == 6) {
		if (ballMultiply >= 120) {
			ballNumber++;
		}
	}
	//If the ball is greater than 189.f run victory method and remove ball effect
	if (ballMultiply >= 189.f) {
		victory();
		showFinalBall = false;
	}
}

//Spawn blank ball at specific location
void example_layer::spawnBlankBalls(glm::vec3 pos) {
	//Sets size and physical properties of ball
	float radius = 0.3f;
	engine::ref<engine::sphere>blank_shape = engine::sphere::create(10, 20, radius);
	engine::game_object_properties blank_props;
	blank_props.position = { pos };
	blank_props.meshes = { blank_shape->mesh() };
	blank_props.type = 1;
	blank_props.bounding_shape = glm::vec3(radius);
	blank_props.restitution = 0.92f;
	//Sets mass very high so it does not move when collided with
	blank_props.mass = 10000000.1f;
	m_blank = engine::game_object::create(blank_props);

	//Adds to game object vector
	m_game_objects.push_back(m_blank);
	storingBlankBalls.push_back(m_blank);
}

//Method to spawn boss
void example_layer::bossSpawn(glm::vec3 pos) {
	//Spawns boss at specific size and location
	engine::ref<engine::model> m_boss_mesh = engine::model::create("assets/models/static/lich/final.obj");
	engine::game_object_properties boss_props;
	boss_props.meshes = m_boss_mesh->meshes();
	boss_props.textures = m_boss_mesh->textures();
	float boss_scale = 1.f / glm::max(m_boss_mesh->size().x, glm::max(m_boss_mesh->size().y, m_boss_mesh->size().z));
	boss_props.position = { pos };
	boss_props.scale = glm::vec3(boss_scale) * 1.5f;
	boss_props.bounding_shape = m_boss_mesh->size() / 2.f;
	m_boss = engine::game_object::create(boss_props);
	m_boss->set_offset(m_boss_mesh->offset());
	m_boss->set_mass(10000.f);

	//Initialise the boss class
	theBoss.initialise(m_boss, pos,glm::vec3(0.0f, 0.0f, 0.0f));
	//Create boss box
	theBoss.set_box(boss_props.bounding_shape.x * 2.f * 1.5f * boss_scale, boss_props.bounding_shape.y * 2.f * 1.5f * boss_scale, boss_props.bounding_shape.z * 2.f * 1.5f * boss_scale, boss_props.position - glm::vec3(0.f, m_boss->offset().y, 0.f) * m_boss->scale());
	//Add to game objects vector
	m_game_objects.push_back(m_boss);
	//Records that the boss has spawned
	bossHasSpawned = true;

}

//Method to spawn rat
void example_layer::ratSpawn(glm::vec3 pos) {
	//Spawns rat at specific size and location
	engine::ref <engine::model> rat_model = engine::model::create("assets/models/static/sized1.obj");
	engine::game_object_properties rat_props;
	rat_props.meshes = rat_model->meshes();
	rat_props.textures = rat_model->textures();
	float rat_scale = 1.f / glm::max(rat_model->size().x, glm::max(rat_model->size().y, rat_model->size().z));
	rat_props.scale = glm::vec3(rat_scale) * 4.0f;
	rat_props.bounding_shape = rat_model->size() / 2.f;
	rat_props.position = { pos };
	rat_props.rotation_axis = glm::vec3(0.f, 0.f, 1.f);

	std::shared_ptr<engine::game_object> m2_rat = std::make_shared<engine::game_object>(rat_props);
	m2_rat->set_offset(rat_model->offset());
	m2_rat->set_mass(0.1f);
	//Initialise the rat class
	rat theRat;
	theRat.initialise(m2_rat, pos, glm::vec3(1.f, 0.f, 0.f));
	//Create rat box
	theRat.set_box(rat_props.bounding_shape.x * 8.f * rat_scale, rat_props.bounding_shape.y * 8.f * rat_scale, rat_props.bounding_shape.z * 8.f * rat_scale, rat_props.position - glm::vec3(0.f, m2_rat->offset().y, 0.f) * m2_rat->scale());
	//Add to game objects vector
	m_game_objects.push_back(m2_rat);
	storingRatModels.push_back(m2_rat);

	storingRats.push_back(theRat);
	storingRatGameObjectPos.push_back(m_game_objects.size());

	//Records that the rat is active
	storingratbool.push_back(true);
	ratActive.push_back(true);

	//Increases the number of rats
	ratNumber += 1;
}

//Method to spawn plasma at specific location
void example_layer::spawnPlasma(glm::vec3 pos) {
	std::shared_ptr<plasma> current = std::make_shared<plasma>(glm::vec3(1.f, 0.f, 1.0f), glm::vec3(0.f, 0.f, 1.f), 0.25f, 1.0f);
	current->activate(0.13f, pos);
	storingPlasma.push_back(current);
	lastPlasmaPosition.push_back(glm::vec3(m_player.getThePosition().x, 1.1f, m_player.getThePosition().z));
	heldPlasma.push_back(current);
}

//Method to spawn plasma ball at specific location
void example_layer::spawnPlasmaBall(glm::vec3 pos) {
	//Creates sphere with physical properties
	float radius = 0.13f;
	engine::ref<engine::sphere>plasma_shape = engine::sphere::create(15, 30, radius);
	engine::game_object_properties plasma_props;
	plasma_props.position = { pos };
	plasma_props.meshes = { plasma_shape->mesh() };
	plasma_props.type = 1;
	plasma_props.bounding_shape = glm::vec3(radius);
	plasma_props.restitution = 0.92f;
	plasma_props.mass = 0.f;
	this_plasma = engine::game_object::create(plasma_props);

	//Adds to game objects
	m_game_objects.push_back(this_plasma);
	//Records plasma ball
	storingPlasmaBalls.push_back(this_plasma);
	plasmaCanAffect.push_back(true);
	//Records the plasma ball's location and the time remaining
	storingPlasmaBallLocation.push_back(m_game_objects.size());
	lastPlasmaBallPosition.push_back(pos);
	availablePlasmaBall.push_back(false);
	plasmaBallTimeRemaining.push_back(1.9f);

	//Increases number of plasma balls
	numberOfPlasmaBalls += 1;
}

//Method to create a plasma light
void example_layer::createPlasmaLight(glm::vec3 pos) {
	//Create a purple point light with given values at a set position
	engine::PointLight thepPlasmaLight;
	thepPlasmaLight.Color = glm::vec3(1.0f, 0.0f, 1.0f);
	thepPlasmaLight.AmbientIntensity = 0.0f;
	thepPlasmaLight.DiffuseIntensity = 0.0f;
	thepPlasmaLight.Position = glm::vec3(pos.x, 1.0f, pos.z);
	std::shared_ptr<engine::PointLight> current = std::make_shared<engine::PointLight>(thepPlasmaLight);
	//Store the light location details
	storingPlasmaLights.push_back(current);
	lastPlasmaLightPosition.push_back(glm::vec3(pos.x, 1.1f, pos.z));
	//Record the time remaining for each plasma light
	plasmaTimeRemaining.push_back(2.0f);
	availablePlasma.push_back(false);
	//Increases the number of lights
	num_point_lights += 1;
}

//Method to create boss plasma at a specific location
void example_layer::spawnBossPlasma(glm::vec3 pos) {
	std::shared_ptr<plasma> current = std::make_shared<plasma>(glm::vec3(1.f, 0.f, 0.0f), glm::vec3(0.f, 0.f, 0.f), 0.25f, 1.0f);
	current->activate(0.13f, pos);
	storingBossPlasma.push_back(current);
	lastBossPlasmaPosition.push_back(glm::vec3(m_boss->position().x, 1.1f, m_boss->position().z));
	current->release();
}

//Method to create a boss plasma ball
void example_layer::spawnBossPlasmaBall(glm::vec3 pos) {
	//Sets physical propoerties for the ball
	float radius = 0.13f;
	engine::ref<engine::sphere>plasma_shape = engine::sphere::create(15, 30, radius);
	engine::game_object_properties plasma_props;
	plasma_props.position = { pos };
	plasma_props.meshes = { plasma_shape->mesh() };
	plasma_props.type = 1;
	plasma_props.bounding_shape = glm::vec3(radius);
	plasma_props.restitution = 0.92f;
	plasma_props.mass = 0.f;
	this_plasma = engine::game_object::create(plasma_props);
	//Adds to game objects
	m_game_objects.push_back(this_plasma);
	//Records the boss plasma ball and whether or not it can cause collisions
	storingBossPlasmaBalls.push_back(this_plasma);
	bossPlasmaCanAffect.push_back(true);
}

