#include "pch.h"
#include "boss.h"
#include <iostream>
#include <algorithm>
boss::boss() {}
boss::~boss() {}
void boss::initialise(engine::ref<engine::game_object> object, glm::vec3 position, glm::vec3 forward)
{
	//Sets the initial position and forward direction
	m_object = object;
	m_object->set_forward(forward);
	m_object->set_position(position);
}
void boss::on_update(const engine::timestep& time_step, const glm::vec3& player_position, bool startMethods)
{
	if (startMethods == true) {
		if (health <= 0) {
			//If health runs out, set killed to true and move boss off map
			isKilled = true;
			m_object->set_position(glm::vec3(0.0f, -100.0f, 0.0f));
		}
		else {
			//Calculates distance between the player and boss
			float distance_to_player = glm::distance(m_object->position(), player_position);
			//If player is not in boss area
			if (checkBossArea(player_position) == true) {
				//Return boss to throne and start guarding
				isEvading = false;
				guarding(time_step, player_position);
				m_state = state::guard;
			}
			else {
				//If player is too close run away
				if (distance_to_player <= m_too_close_radius) {
					isEvading = false;
					away_from_player(time_step, player_position);
					m_state = state::away_from_player;
				}
				else {
					//If health is less than zero, run to health
					if (health <= 40) {
						isEvading = false;
						get_health(time_step, player_position);
						m_state = state::get_health;
					}
					else {
						//If player is too far then move closer
						if (distance_to_player >= m_too_far_radius) {
							isEvading = false;
							move_to_player(time_step, player_position);
							m_state = state::move_to_player;
						}
						//If player is not too close or far, then evade left to right to dodge shots
						else {
							evade_player(time_step, player_position);
							m_state = state::evade_player;
						}
					}
				}
			}
			//Updates the player collision box
			m_boss_box.on_update(object()->position() - glm::vec3(0.f, object()->offset().y, 0.f) * object()->scale(), object()->rotation_amount(), object()->rotation_axis());
		}
	}

}
//If the player is too far away, move closer to the player
void boss::move_to_player(const engine::timestep& time_step, const glm::vec3& player_position) {
	m_speed = 0.8f;

	//Angle boss towards player and move to the player
	m_object->set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));
	m_object->set_forward(player_position - m_object->position());
	m_object->set_position(m_object->position() + m_object->forward() * m_speed * (float)time_step);
	m_object->set_rotation_amount(atan2(-m_object->forward().x, -m_object->forward().z));

	//Sets the direction to shoot in
	shootingDirection = (player_position - m_object->position());

	//Shoot every two seconds, if not reduce timer by time_step
	if (shootingDelay <= 0.0f) {
		shoot = true;
		shootingDelay = 2.0f;
	}
	else {
		shootingDelay -= 1.0f * time_step;
	}
}
//If player is too close, move away
void boss::away_from_player(const engine::timestep& time_step, const glm::vec3& player_position) {
	m_speed = 1.0f;
	//Angle boss towards player and move in the opposite direction
	m_object->set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));
	m_object->set_forward(player_position - m_object->position());
	m_object->set_position(m_object->position() + m_object->forward() * -1.f * m_speed * (float)time_step);
	m_object->set_rotation_amount(atan2(-m_object->forward().x, -m_object->forward().z));

	//Sets the direction to shoot in
	shootingDirection = (player_position - m_object->position());

	//Shoot every two seconds, if not reduce timer by time_step
	if (shootingDelay <= 0.0f) {
		shoot = true;
		shootingDelay = 2.0f;
	}
	else {
		shootingDelay -= 1.0f * time_step;
	}
}
void boss::evade_player(const engine::timestep& time_step, const glm::vec3& player_position) {
	//If boss is currently evading
	if (isEvading == true) {
		//Stores the up vector
		glm::vec3   m_world_up_vector{ 0.f,1.f,0.f };

		//Uses the cross product to calculate the left and right vector
		glm::vec3 m_right_vector = glm::normalize(glm::cross(m_object->forward(), m_world_up_vector));
 
		//Left
		if (evadeDirection == 0) {
			m_speed = 4.f;
			//Moves player towards left direction
			m_object->set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));
			m_object->set_forward(player_position - m_object->position());
			m_object->set_rotation_amount(atan2(-m_object->forward().x, -m_object->forward().z));
			m_object->set_position(m_object->position() + m_speed * time_step * m_right_vector);
			shootingDirection = (player_position - m_object->position());
		}
		//Right
		else {
			m_speed = 4.f;
			//Moves player towards right direction
			m_object->set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));
			m_object->set_forward(player_position - m_object->position());
			m_object->set_rotation_amount(atan2(-m_object->forward().x, -m_object->forward().z));
			m_object->set_position(m_object->position() - m_speed * time_step * m_right_vector);
			shootingDirection = (player_position - m_object->position());
		}
		//Reduce the timer 
		evadingTimer -= 1.0f * time_step;

		//If timer runs out, stop evading
		if (evadingTimer <= 0.0f) {
			isEvading = false;
		}
	}
	else {
		//Calculates a new evade direction and starts again
		evadeDirection = rand() % 2;
		isEvading = true;
		evadingTimer = 0.6f;
	}
	//Shoot every two seconds, if not reduce timer by time_step
	if (shootingDelay <= 0.0f) {
		shoot = true;
		shootingDelay = 2.0f;
	}
	else {
		shootingDelay -= 1.0f * time_step;
	}
}
void boss::get_health(const engine::timestep& time_step, const glm::vec3& player_position) {
	//Calculates the closest health pack and sets that to the goal
	if(runningToHealth == false){
		float distance_to_health_1 = glm::distance(m_object->position(), healthPack1Pos);
		float distance_to_health_2 = glm::distance(m_object->position(), healthPack2Pos);

		if (distance_to_health_1 <= distance_to_health_2) {
			healthPackGoal = healthPack1Pos;
		}
		else {
			healthPackGoal = healthPack2Pos;
		}
		runningToHealth = true;
	}
	else {
		//If the boss reaches the health pack, stop running to the health pack and increase health by 40
		if (glm::distance(m_object->position(), healthPackGoal) <= 1.0f) {
			runningToHealth = false;
			health += 40;
		}
		//Moves towards the health pack goal
		else {
			m_speed = 0.8f;
			m_object->set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));
			m_object->set_forward(healthPackGoal - m_object->position());
			m_object->set_position(m_object->position() + m_object->forward() * m_speed * (float)time_step);
			m_object->set_rotation_amount(atan2(-m_object->forward().x, -m_object->forward().z));
		}
	}
}

//Guarding state, the boss returns to the guard position in front of throne
void boss::guarding(const engine::timestep& time_step, const glm::vec3& player_position) {
	m_speed = 1.0f;
	m_object->set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));
	m_object->set_forward(guardPosition - m_object->position());
	m_object->set_position(m_object->position() + m_object->forward() * 1.f * m_speed * (float)time_step);
	m_object->set_rotation_amount(atan2(-m_object->forward().x, -m_object->forward().z));
}

//Checks if the player is within the boss area
bool boss::checkBossArea(const glm::vec3& player_position) {
	if (player_position.z <= 4.8f) {
		return true;
	}
	else if (player_position.x >= 0.0f) {
		return true;
	}

	return false;
}

