#pragma once
#include <engine.h>
#include "player.h"
#include "vector"
#include "intro.h"
#include "controls.h"
#include "quest.h"
#include "soulHolder.h"
#include "healthBar.h"
#include "plasma.h"
#include "emitter.h"
#include "spellCircle.h"
#include "onFire.h"
#include "died.h"
#include "engine/entities/bounding_box_bullet.h"
#include "rat.h"
#include "boss.h"
//Relevant Classes
class health;
class soul;
class diamond;
class man;
class blade;
class hilt;
class shield;

class example_layer : public engine::layer
{
public:
    example_layer();
	~example_layer();

    void on_update(const engine::timestep& time_step) override;
    void on_render() override; 
    void on_event(engine::event& event) override;

	//Method to check whether to run the 3rd person camera or map
	void minimap();

	//Variable to hold the current quest
	std::string info = "";

	//Method to spawn crosses at specific locations and with specific rotations
	void crossSpawn(const::engine::ref<engine::shader>, glm::vec3, float);

	//Method to spawn souls (diamonds) at specific locations
	void diamondSpawn(const::engine::ref<engine::shader>, glm::vec3);

	//Method used to spawn the trio's of swords at specific locations
	void swordSpawn(const::engine::ref<engine::shader>, glm::vec3, float, glm::vec3, glm::vec3, float);

	//Method to spawn sword laying on the ground at specific locations
	void flatSwordSpawn(const::engine::ref<engine::shader>, glm::vec3, float);

	//Method to spawn shields laying on the ground at specific locations
	void flatShieldSpawn(const::engine::ref<engine::shader>, glm::vec3, float);

	//Sets the current state of the health bar based on the player's current health
	void setHealthBarState();

	//Method to handle the main game timer
	void theTimer(const engine::timestep& time_step);

	//Method to update the health bars
	void updateHealthBars();

	//Variable to hold the current timer value to display
	std::string timer = "";

	//Method to spawn health packs at random locations
	void spawnHealthPack();

	void spawnBossHealthPack();

	//Method to spawn souls at specific locations, will drop on NPC death
	void spawnSouls(glm::vec3);

	//Method to start setting the player on fire, starting the animation and beginning the timer
	void setOnFire();

	//Method to count down the time the player is on fire, to reduce health over the course of the time
	void fireHit(const engine::timestep&);

	//Method to count down as soul's exist to remove them after a few seconds
	void reduceDiamondTimes(const engine::timestep&);

	//Method to activate the victory animation
	void victory();

	//Method to activate the defeat animation
	void defeat();

	//Method to grow the spheres used in the victory animation to have them cover the screen
	void growBall(const engine::timestep&);

	//Method to create point lights within the plasma balls
	void createPlasmaLight(glm::vec3);

	//Method to spawn the plank balls used for collision detection in the fire wall
	void spawnBlankBalls(glm::vec3);

	//Method to spawn the boss
	void bossSpawn(glm::vec3);

	//Method to spawn the rat
	void ratSpawn(glm::vec3);

	//Method to spawn plasma
	void spawnPlasma(glm::vec3);

	//Method to spawn the plasma ball
	void spawnPlasmaBall(glm::vec3);

	//Method to spawn boss plasma
	void spawnBossPlasma(glm::vec3);

	//Method to spawn boss plasma ball
	void spawnBossPlasmaBall(glm::vec3);

private:
	engine::ref<engine::skybox>			m_skybox{};
	engine::ref<engine::game_object>	m_terrain{};
	engine::ref<engine::game_object>	m_mannequin{};
	engine::ref<soul>					m_soul{};
	player								m_player{};
	engine::ref<engine::game_object>	m_dude{};
	engine::ref<engine::game_object>	m_throne{};
	engine::ref<engine::game_object>	m_cross{};
	engine::ref<engine::material>		m_material{};
	engine::ref<engine::material>		m_mannequin_material{};
	engine::DirectionalLight            m_directionalLight;
	std::vector<engine::ref<engine::game_object>>     m_game_objects{};
	engine::ref<engine::bullet_manager> m_physics_manager{};
	engine::ref<engine::audio_manager>  m_audio_manager{};
	float								m_prev_sphere_y_vel = 0.f;
	engine::ref<engine::text_manager>	m_text_manager{};
    engine::orthographic_camera			m_2d_camera; 
    engine::perspective_camera			m_3d_camera;
	engine::ref<health>					m_health{};
	engine::ref<engine::heightmap>		m_heightmap;
	engine::ref<engine::game_object>	m_diamond{};
	engine::ref<engine::material>		m_diamond_material{};
	engine::ref<engine::game_object>	m_blade{};
	engine::ref<engine::material>		m_blade_material{};
	engine::ref<engine::game_object>	m_hilt{};
	engine::ref<engine::material>		m_hilt_material{};
	engine::ref<engine::game_object>	m_shield{};
	engine::ref<engine::material>		m_shield_material{};
	engine::ref<engine::game_object>	m_trim{};
	engine::ref<engine::material>		m_trim_material{};
	engine::ref<intro>					m_intro{};
	engine::ref<controls>				m_controls{};
	engine::ref<quest>					m_quest{};
	engine::ref<onFire>					m_on_fire{};
	engine::ref<died>					m_died{};
	engine::ref<died>					m_lose{};

	//References for each of the soul holder states
	engine::ref<soulHolder>					m_soulHolder0{};
	engine::ref<soulHolder>					m_soulHolder1{};
	engine::ref<soulHolder>					m_soulHolder2{};
	engine::ref<soulHolder>					m_soulHolder3{};
	engine::ref<soulHolder>					m_soulHolder4{};
	engine::ref<soulHolder>					m_soulHolder5{};
	engine::ref<soulHolder>					m_soulHolder6{};
	engine::ref<soulHolder>					m_soulHolder7{};

	//References for each of the health bar states
	engine::ref<healthBar>					m_healthBar0{};
	engine::ref<healthBar>					m_healthBar5{};
	engine::ref<healthBar>					m_healthBar10{};
	engine::ref<healthBar>					m_healthBar15{};
	engine::ref<healthBar>					m_healthBar20{};
	engine::ref<healthBar>					m_healthBar25{};
	engine::ref<healthBar>					m_healthBar30{};
	engine::ref<healthBar>					m_healthBar35{};
	engine::ref<healthBar>					m_healthBar40{};
	engine::ref<healthBar>					m_healthBar45{};
	engine::ref<healthBar>					m_healthBar50{};
	engine::ref<healthBar>					m_healthBar55{};
	engine::ref<healthBar>					m_healthBar60{};
	engine::ref<healthBar>					m_healthBar65{};
	engine::ref<healthBar>					m_healthBar70{};
	engine::ref<healthBar>					m_healthBar75{};
	engine::ref<healthBar>					m_healthBar80{};
	engine::ref<healthBar>					m_healthBar85{};
	engine::ref<healthBar>					m_healthBar90{};
	engine::ref<healthBar>					m_healthBar95{};
	engine::ref<healthBar>					m_healthBar100{};

	//Reference for the man model
	engine::ref<man>						m_man{};

	//Reference for the m_plasma
	engine::ref<plasma>						m_plasma{};

	//Reference for the this_plasma
	engine::ref<engine::game_object>		this_plasma{};

	//Record the plasma
	std::vector<std::shared_ptr<plasma>>	storingPlasma;

	//Record the last plasma location
	std::vector<glm::vec3>					lastPlasmaPosition;

	//Record the plasma being held
	std::vector<std::shared_ptr<plasma>>	heldPlasma;

	//Record the plasma balls
	std::vector<engine::ref<engine::game_object>>		storingPlasmaBalls{};

	//Record if plasma is being held
	bool isHoldingPlasma;

	//Vector recording if the plasma balls can cause collisions
	std::vector<bool>						plasmaCanAffect;

	//Record of boss plasma
	std::vector<std::shared_ptr<plasma>>	storingBossPlasma;

	//Record the last boss plasma position
	std::vector<glm::vec3>					lastBossPlasmaPosition;

	//Record the boss plasma ball
	std::vector<engine::ref<engine::game_object>>		storingBossPlasmaBalls{};

	//Vector to record if the boss plasma ball can cause collisions
	std::vector<bool>						bossPlasmaCanAffect;

	//Vector to store pointers to the health packs in the game so they can be accessed
	std::vector<std::shared_ptr<health>>	storingHealth;

	std::vector<std::shared_ptr<health>>	storingBossHealth;

	//Vector to store pointers to the souls in the game so they can be accessed
	std::vector<std::shared_ptr<soul>>		storingSouls;

	//Records the time left for the souls
	std::vector<float>						soulTimer;

	//Creates the ball game objects
	engine::ref<engine::game_object>		m_ball{};
	engine::ref<engine::game_object>		m_ball2{};
	engine::ref<engine::material>			m_lightsource_material{};

	//The counter to hold the time between creating new balls
	float ballMultiply = 1.0f;

	//How many balls have been created
	int ballNumber = 1;

	//Whether or not to show the final ball
	bool showFinalBall = false;

	//The spell circle
	spellCircle m_spell_circle;

	//Setting the terrain height so the heightmap matches up
	float m_physical_terrain_height = 0.5f;

	//Variable to keep track of whether or not the intro screen is being shown, along with relevant getter and setter methods
	bool isIntro = false;

	void setIsIntro(bool y) {
		isIntro = y;
	}

	bool getIsIntro() {
		return isIntro;
	}

	//Variable to keep track of whether or not the controls screen is being shown, along with relevant getter and setter methods
	bool isControls = true;

	void setIsControls(bool y) {
		isControls = y;
	}

	bool getIsControls() {
		return isControls;
	}

	//Variable to keep track of whether or not the quest screen is being shown, along with relevant getter and setter methods
	bool isQuest = false;

	void setIsQuest(bool y) {
		isQuest = y;
	}

	bool getIsQuest() {
		return isQuest;
	}

	//Variable to keep track of whether or not the map is being shown, along with relevant getter and setter methods
	bool isMap = false;

	void setIsMap(bool y) {
		isMap = y;
	}
	bool getIsMap() {
		return isMap;
	}

	//Variable to hold the verticies of the diamond when creating the soul
	std::vector<glm::vec3> diamond_vertices;

	//Variable to hold the verticies of the shield
	std::vector<glm::vec3> shield_vertices;

	//Variable to keep track of whether or not the first quest has been delivered so it does not keep appearing
	bool hasQuest1 = false;

	//Variable to hold the distance between the player and warrior to calculate when the player is close enough to 'speak to the warrior'
	glm::vec3 warriorLength;

	//Variable to hold which quest is active
	//Quest 0: "Find one of your soldiers to figure out what has happened"
	//Quest 1: "Collect 6 souls from monsters"
	//Quest 2: "Cross the cursed flames"
	//Quest 3: "Slay the evil sorcerer"
	int questNumber;

	//Variable to determine whether the quest should be shown (is hidden during info screens)
	bool hideQuest = true;

	//Variable to store if the controls screen is the first instance
	bool isInitial = true;

	//Amount of souls the user has collected
	int soulAmount = 0;

	//Amount of time left in the game
	float counter = 500000;

	//Variable to decide when to start the main game timer
	bool startMainTimer = false;

	//Variable to hold whether the timer should be shown
	bool showMainTimer = true;

	//To hold position of fallen warrior
	glm::vec3 dudePosition;

	//The particle emmitter for flame effect
	emitter theFlame;

	//How many more times the fire timer should run
	int fireCounter;

	//How many seconds are remaining in fire timer
	float fireCounting;

	//Stores whether fire timer should run
	bool fireTrigger = false;

	//Records the plasma lights
	std::vector<std::shared_ptr<engine::PointLight>> storingPlasmaLights;

	//Records the last plasma light locations
	std::vector<glm::vec3>			lastPlasmaLightPosition;

	//Records the time remaining for the plasma light
	std::vector<float>				plasmaTimeRemaining;

	//Records if a plasma ball is available to be reused
	std::vector<bool>				availablePlasma;

	//Records the plasma ball time remaining
	std::vector<float>				plasmaBallTimeRemaining;

	//Records if the plasma ball is availble to be reused
	std::vector<bool>				availablePlasmaBall;

	//Records the last plasma ball location
	std::vector<glm::vec3>			lastPlasmaBallPosition;

	//Records the number of plasma balls
	int numberOfPlasmaBalls = 0;

	//Stores the current plasma ball location
	std::vector<int>	storingPlasmaBallLocation;

	//Creates the final light
	engine::PointLight m_finalLight;
	uint32_t num_final_lights = 1;


	//Creates the spotlight
	engine::SpotLight m_spotLight;
	uint32_t num_spot_lights = 1;

	//Records whether the blank balls are to be rendered
	bool blankShow = false;

	//Creates a blank game object
	engine::ref<engine::game_object>		m_blank{};

	//Vector to store the blank balls used in the fire effect so they can be used for collision detection
	std::vector<engine::ref<engine::game_object>>		storingBlankBalls{};

	//Variable to hold whether or not the player is on fire (so effect isn't repeated while already active)
	bool whileOnFire = false;

	//Creates the 6 point lights
	engine::PointLight					m_pointLight1;
	engine::PointLight					m_pointLight2;
	engine::PointLight					m_pointLight3;
	engine::PointLight					m_pointLight4;
	engine::PointLight					m_pointLight5;
	engine::PointLight					m_pointLight6;
	engine::PointLight					m_pointLight7;
	engine::PointLight					m_pointLight8;

	//Holds the current genrerated point lights
	uint32_t							num_point_lights = 0;

	//Records the time left to strafe
	float								strafeBack;

	//Records if the player is strafing
	bool								isStrafing = false;

	//Stores the rats
	std::vector<rat>									storingRats;

	//Stores the rat's position in the game objects vector
	std::vector<int>									storingRatGameObjectPos;

	//Vector of rat models to render
	std::vector<std::shared_ptr<engine::game_object>>	storingRatModels;

	//Time remaining between rat spawns
	float ratSpawnRemaining = 6.0f;

	//Stores whether rat can spawn
	bool ratCanSpawn = true;

	//Stores whether the rat is able to cause collisions
	std::vector<bool>	storingratbool;
	std::vector<bool>	ratActive;

	//The boss object
	boss								theBoss{};

	//The boss game object
	engine::ref<engine::game_object>	m_boss{};

	//Method to record if the boss has started being active
	bool bossHasSpawned = false;
	bool startBossMethods = false;

	//Amount of time remaining for rat damage
	float ratDamageCounter = 0.0f;

	//Records the number of rats
	int ratNumber = 0;

	//Amount of time remaining for ghost audio so it can loop every 6 seconds
	float ghostCounter = 0.0f;

	//Records if the laugh sound has started playing on death
	bool isLaughing = false;

	//Records if the laugh sound has started playing on defeat
	bool isDefeatLaughing = false;

	//Records if the player has won the game
	bool hasVictory = false;

	//Records if the player has been defeated
	bool hasDefeat = false;

	//Records if the defeat methods have started
	bool defeatStarted = false;

	//Holds whether or not the 4th quest has been given
	bool hasQuest4Shown = false;

};
