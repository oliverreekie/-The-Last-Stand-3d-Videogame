#pragma once
#include <engine.h>
class rat
{
	//Holds the state of the rat
	enum class state
	{
		chasing,
		searching,
		attack,
		flee,
		curious
	};
public:
	rat();
	~rat();
	//Set the parameters for the rat
	void initialise(engine::ref<engine::game_object> object, glm::vec3 position,
		glm::vec3 forward);
	//Update the rat
	void on_update(const engine::timestep& time_step, const glm::vec3& player_position);

	//Rat state behaviour methods
	void chase_player(const engine::timestep& time_step, const glm::vec3& player_position);
	void search_player(const engine::timestep& time_step, const glm::vec3& player_position);
	void attack_player(const engine::timestep& time_step, const glm::vec3& player_position);
	void flee_player(const engine::timestep& time_step);
	void curious(const engine::timestep& time_step);

	//Game object bound to the rat
	engine::ref<engine::game_object> object() const { return m_object; }

	//Set the rat bounding box
	void set_box(float width, float height, float depth, glm::vec3 position) { m_rat_box.set_box(width, height, depth, position); }
	engine::bounding_box& getBox() { return m_rat_box; };

	//Get if the rat is to be removed
	bool getToRemove() {
		return toRemove;
	}
	//Decrease rat health
	void decreaseHealth(int h) {
		health -= h;
	}
	//Get rat health
	bool getIsKilled() {
		return isKilled;
	}
	//Set rat to alive
	void setAlive(){
		//Sets the killed and removed states to false
		toRemove = false;
		isKilled = false;
		//Sets health to full
		health = 100;
		//Sets initial state to search
		state m_state = state::searching;
		isFleeing = false;
	}
	//Sets position of rat
	void setPosition(glm::vec3 p) {
		m_object->position() = p;
	}

	//Returns rat position
	glm::vec3 getPosition() {
		return m_object->position();
	}
	//Set curious state
	void setIsCurious(bool b) {
		isCurious = b;
	}
	//Set the curious goal
	void setCuriousGoal(glm::vec3 g) {
		curiousGoal = g;
	}

private:
	//Rat's speed
	float m_speed{ 0.5f };

	//Timer controlling the direction switch and the reset value for this timer
	float m_default_time{ 4.f };
	float m_switch_direction_timer = m_default_time;

	//Rat vision distances
	float m_see_radius = 12.f;
	float m_attack_radius = 3.f;
	// game object bound to the rat
	engine::ref< engine::game_object> m_object;
	//current state of the rat's state machine
	state m_state = state::searching;

	//Rat collision box
	engine::bounding_box	m_rat_box;

	//Rat health
	int health = 100;

	//Whether the rat is searching
	bool headedToSearch = false;

	//Where the rat is going to search
	glm::vec3 placeToSearch;

	//List of possible flee points
	glm::vec3 fleePoint1 = glm::vec3(41.5f, 1.0f, 0.1f);
	glm::vec3 fleePoint2 = glm::vec3(18.6f, 1.0f, 25.7f);
	glm::vec3 fleePoint3 = glm::vec3(-14.f, 1.0f, -12.2f);
	glm::vec3 fleePoint4 = glm::vec3(-32.5f, 1.0f, -38.8f);

	//Rat flee goal
	glm::vec3 fleeGoal;

	//Whether the rat is fleeing
	bool isFleeing = false;

	//Sets the rat to be removed
	bool toRemove = false;

	//Sets the rat to be killed
	bool isKilled = false;

	//Delay between attacks
	float attackDelay = 0.0f;

	//List of possible rat spawn points
	glm::vec3 ratSpawn1 = glm::vec3(7.5f, 1.f, -40.3f);
	glm::vec3 ratSpawn2 = glm::vec3(-25.2f, 1.f, -11.4f);
	glm::vec3 ratSpawn3 = glm::vec3(42.7f, 1.f, -4.7f);
	glm::vec3 ratSpawn4 = glm::vec3(40.6f, 1.f, -2.2f);
	glm::vec3 ratSpawn5 = glm::vec3(38.9f, 1.f, 17.5f);
	glm::vec3 ratSpawn6 = glm::vec3(13.8f, 1.f, 12.7f);

	//Sets if the rat is curious
	bool isCurious = false;

	//The goal that the rat is running to when curious
	glm::vec3 curiousGoal;

};
