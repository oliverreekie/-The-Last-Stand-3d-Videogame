#include "player.h"
#include "pch.h"
#include "engine/core/input.h"
#include "engine/key_codes.h"
#include <iostream>


player::player() : m_timer(0.0f), m_speed(0.0f)
{
	;
}

player::~player()
{
}


void player::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
	//Set the initial orientation so the player is looking towards the center of the map
	m_object->set_forward(glm::normalize(glm::vec3(-35.f, 0.0, 35.f)));
	//Sets the player location to the spawn area
	m_object->set_position(glm::vec3(38.f, 0.5, -40.f));
	//Stores the player position so proximity to the fallen warrior can be calculated
	thePosition = glm::vec3(38.f, 0.5, -40.f);
	m_object->animated_mesh()->set_default_animation(2);
	
}
void player::on_update(const engine::timestep& time_step)
{
	//m_object->set_position(m_object->position() += m_object->forward() * m_speed *
	//	(float)time_step);
	thePosition = (m_object->position() += m_object->forward() * m_speed *(float)time_step);
	//m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
	theDirection = m_object->forward();
	float down_vel = m_object->velocity().y;
	m_object->set_velocity(glm::vec3(m_object->forward().x * m_speed, down_vel, m_object->forward().z * m_speed));

	m_object->animated_mesh()->on_update(time_step);

	if (isDeath == false) {
		if (engine::input::key_pressed(engine::key_codes::KEY_A)) {
			//Turns the player to the left
			turn(1.0f * time_step);
		}
		else if (engine::input::key_pressed(engine::key_codes::KEY_D)) {
			//Turns the player to the right
			turn(-1.0f * time_step);
		}
		else
			turn(0.0f);
	}
	else if (isDeath == true) {
		//If player dies, rotate and fall to the ground
		m_object->set_forward(glm::rotate(m_object->forward(), 0.20f * time_step, glm::vec3(0.f, 0.f,
			1.f)));
	}
	//Updates player collision box
	m_player_box.on_update(object()->position() - glm::vec3(0.f, object()->offset().y, 0.f) * object()->scale(), object()->rotation_amount(), object()->rotation_axis());

}

void player::turn(float angle) {
	//Angle the player to the left or right depending on input
	m_object->set_angular_velocity(glm::vec3(0.f, angle * 100.f, 0.f));
}
//Function to create 3rd person player camera
void player::update_camera(engine::perspective_camera& camera) {
	//Sets the camera to a set distance behind the player
	glm::vec3 cam_pos = m_object->position() - glm::normalize(m_object->forward()) * 3.5f;
	cam_pos.y += 1.7f;
	//Sets the camera to look down over the camera to provide the best view
	glm::vec3 cam_look_at = m_object->position() + glm::normalize(m_object->forward()) * 6.f;
	camera.set_view_matrix(cam_pos, cam_look_at);
}
//Function to create the map camera
void::player::newCam(engine::perspective_camera& camera) {
	//Sets the camera to be above the player
	glm::vec3 cam_pos = m_object->position() - glm::normalize(m_object->forward()) * 6.f;
	//Sets the camera height above the ground
	cam_pos.y += 30.f;
	//Sets the camera to look down at the player
	glm::vec3 cam_look_at = m_object->position();
	camera.set_view_matrix(cam_pos, cam_look_at);
}
//Function to set the player's animation to still
void player::setStill() {
	m_object->animated_mesh()->switch_animation(2);
}
//Function to set the player's speed
void player::set_m_speed(float f) {
	m_speed = f;
}
//Function to store whether the player is walking
void player::setWalk(bool b) {
	isWalk = b;
}
//Function to store whether the player is running
void player::setRunning(bool b) {
	isRunning = b;
}
//Sets the animation of the player
void player::setAnimation(int i) {
	m_object->animated_mesh()->switch_animation(i);
}
//Gets the player position, used for calculating the proximity to the fallen warrior
glm::vec3 player::getThePosition() {
	return thePosition;
}
//Get if the player is running
bool player::getIsRunning() {
	return isRunning;
}
//Get if the player is walking
bool player::getIsWalk(){
	return isWalk;
}
//Get the health of the player
int player::getHealth() {
	return health;
}
//Set the player health
void player::setHealth(int i) {
	health = i;
}
//Reduce the player health
void player::reduceHealth(int i) {
	health -= i;
}
//Increase player health
void player::increaseHealth(int i) {
	health += i;
}
//Get the forward direction
glm::vec3 player::getDirection() {
	glm::normalize(theDirection);
	return theDirection;
}
//Start the player death
void player::startDeath() {
	isDeath = true;
	m_object->set_rotation_axis(glm::vec3(0.0f, 0.0f, 1.0f));
}



