#pragma once
#include <engine.h>
#include "glm/gtx/rotate_vector.hpp"
#include "engine/entities/bounding_box_bullet.h"
class player
{

public:
	player();
	~player();
	void initialise(engine::ref<engine::game_object> object);
	void on_update(const engine::timestep& time_step);
	engine::ref<engine::game_object> object() const { return m_object; }
	//Function to turn the player to a given angle
	void turn(float angle);
	//3rd person camera
	void update_camera(engine::perspective_camera& camera);
	//Map camera
	void newCam(engine::perspective_camera& camera);
	//Sets the player to the still position
	void setStill();
	//Set the player speed
	void set_m_speed(float f);
	//Set the player to be walking or running
	void setWalk(bool b);
	void setRunning(bool b);
	//Sets the player's animation
	void setAnimation(int i);
	//Gets the position of the player
	glm::vec3 getThePosition();
	glm::vec3 getDirection();

	//Getter methods for the running and walking variables
	bool getIsRunning();
	bool getIsWalk();

	//Functions to get, set and increment health
	int getHealth();
	void setHealth(int);
	void increaseHealth(int);
	void reduceHealth(int);

	//Function to start the death animation
	void startDeath();

	bool getDeath() {
		return isDeath;
	}

	void set_box(float width, float height, float depth, glm::vec3 position) { m_player_box.set_box(width, height, depth, position); }
	engine::bounding_box& getBox() { return m_player_box; };

private:
	//The player speed
	float m_speed{ 0.f };
	//Variables to hold if the player is running or walking
	bool isWalk = false;
	bool isRunning = false;

	float m_timer;
	engine::ref< engine::game_object> m_object;
	//Position of the player
	glm::vec3 thePosition;

	//Player forward direction
	glm::vec3 theDirection;

	//Health of the player
	int health = 100;

	//Variable to hold whether the death animation has begun
	bool isDeath = false;

	//Player bounding box
	engine::bounding_box	m_player_box;
};
