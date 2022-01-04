#pragma once
#include <engine.h>
class boss
{
	//The states of the boss
	enum class state
	{
		move_to_player,
		away_from_player,
		evade_player,
		get_health,
		guard
	};
public:
	boss();
	~boss();
	//Set the parameters for the boss
	void initialise(engine::ref<engine::game_object> object, glm::vec3 position,
		glm::vec3 forward);

	//Update the boss
	void on_update(const engine::timestep& time_step, const glm::vec3& player_position, bool startMethods);

	//Methods to control the boss state behaviours
	void move_to_player(const engine::timestep& time_step, const glm::vec3& player_position);
	void away_from_player(const engine::timestep& time_step, const glm::vec3& player_position);
	void evade_player(const engine::timestep& time_step, const glm::vec3& player_position);
	void get_health(const engine::timestep& time_step, const glm::vec3& player_position);
	void guarding(const engine::timestep& time_step, const glm::vec3& player_position);

	engine::ref<engine::game_object> object() const { return m_object; }

	//Sets the collision box for the boss
	void set_box(float width, float height, float depth, glm::vec3 position) { m_boss_box.set_box(width, height, depth, position); }

	//Return the boss
	engine::bounding_box& getBox() { return m_boss_box; };


	bool checkBossArea(const glm::vec3& player_position);

	//Decrease the health by value
	void decreaseHealth(int h) {
		health -= h;
	}

	//Set the health to value
	void setHealth(int h) {
		health = h;
	}

	//Return the killed state
	bool getIsKilled() {
		return isKilled;
	}

	//Records the location of each health pack
	void setHealthPack1Pos(glm::vec3 f) {
		healthPack1Pos = f;
	}
	void setHealthPack2Pos(glm::vec3 f) {
		healthPack2Pos = f;
	}

	//Returns the health
	float returnhealth() {
		return health;
	}

	//Get the forward direction of the boss
	glm::vec3 getShootingDirection() {
		return glm::normalize(shootingDirection);
	}

	//Return if the player is shooting
	bool getShoot() {
		return shoot;
	}

	//Set whether the boss is shooting
	void setShoot(bool b) {
		shoot = b;
	}

private:
	//Boss's speed
	float m_speed{ 0.5f };
	//Timer controlling the direction switch and the reset value for this timer
	float m_default_time{ 4.f };
	float m_switch_direction_timer = m_default_time;

	//The bounds of sight for the boss
	float m_too_far_radius = 8.f;
	float m_too_close_radius = 4.f;

	//Game object bound to the boss
	engine::ref< engine::game_object> m_object;

	//Current state of the boss's state machine
	state m_state = state::move_to_player;

	//Collision box for the boss
	engine::bounding_box	m_boss_box;

	//The health of the boss
	int health = 100;

	//Stores whether the boss is killed
	bool isKilled = false;

	//Stores the health pack positions
	glm::vec3 healthPack1Pos;
	glm::vec3 healthPack2Pos;

	//The health pack target when getting health
	glm::vec3 healthPackGoal;

	//Is the boss running to a health pack
	bool runningToHealth = false;

	//The direction the boss is shooting in
	glm::vec3 shootingDirection;

	//Delay between each shot
	float shootingDelay = 2.0f;

	//Whether the boss is shooting or not
	bool shoot = false;

	//Position in front of throne where the boss is set to guard
	glm::vec3 guardPosition = glm::vec3(-26.4f, 1.f, 26.5f);

	//Records if the boss is evading
	bool isEvading = false;

	//The timer holding the amount of time since evading has begun
	float evadingTimer = 0.0f;

	//Whether the boss evades to the left or right
	int evadeDirection = 0;

};
