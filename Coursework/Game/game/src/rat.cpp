#include "pch.h"
#include "rat.h"
#include <iostream>
#include <algorithm>
rat::rat() {}
rat::~rat() {}
void rat::initialise(engine::ref<engine::game_object> object, glm::vec3 position,
	glm::vec3 forward)
{
	m_object = object;
	m_object->set_forward(forward);
	m_object->set_position(position);
}
void rat::on_update(const engine::timestep& time_step, const glm::vec3& player_position)
{
	if (isKilled == true || toRemove == true) {
		glm::vec3 toSpawn = glm::vec3(0.0f, 0.0f, 0.0f);

		//Generates a random number
		int rand1 = rand() % (6);

		//Uses random number to select 1 of 6 spawn points
		if (rand1 == 0) {
			toSpawn = ratSpawn1;
		}
		else if (rand1 == 1) {
			toSpawn = ratSpawn2;
		}
		else if (rand1 == 2) {
			toSpawn = ratSpawn3;
		}
		else if (rand1 == 3) {
			toSpawn = ratSpawn4;
		}
		else if (rand1 == 4) {
			toSpawn = ratSpawn5;
		}
		else if (rand1 == 5) {
			toSpawn = ratSpawn6;
		}

		//Resets health and alive values and moves rat to spawn point
		m_object->set_position(toSpawn);
		isKilled = false;
		toRemove = false;
		health = 100;
		m_state = state::searching;
	}
	else {
		//If health is less than 0, kill rat
		if (health <= 0) {
			isKilled = true;
		}
		float distance_to_player = glm::distance(m_object->position(), player_position);

		if (m_state == state::searching) {
			search_player(time_step, player_position);
			//If health is less than 20, flee
			if (health <= 20) {
				m_state = state::flee;
			}
			//If player is in vision, chase
			else if (distance_to_player < m_see_radius) {
				m_state = state::chasing;
			}
			//If rat is curious, set to curious
			else if (isCurious == true) {
				m_state = state::curious;
			}
		}
		else if (m_state == state::chasing) {
			chase_player(time_step, player_position);
			//If health is less than 20, flee
			if (health <= 20) {
				m_state = state::flee;
			}
			//If player is out of vision, start searching
			else if (distance_to_player > m_see_radius) {
				m_state = state::searching;
			}
			//If player is close enough, attack
			else if (distance_to_player < m_attack_radius) {
				m_state = state::attack;
			}
		}
		//If rat is fleeing, flee
		else if (m_state == state::flee) {
			flee_player(time_step);
		}
		else if (m_state == state::attack) {
			attack_player(time_step, player_position);
			//If health is less than 20, flee
			if (health <= 20) {
				m_state = state::flee;
			}
			//If player is in vision, chase
			else if (distance_to_player > m_attack_radius) {
				m_state = state::chasing;
			}
		}
		else if (m_state == state::curious) {
			curious(time_step);
			//If curious goal is reached, start searching
			if (glm::length(curiousGoal - m_object->position()) < 2.0f) {
				m_state = state::searching;
				isCurious = false;
			}
			//If player is in vision, chase
			else if (distance_to_player < m_see_radius) {
				m_state = state::chasing;
				isCurious = false;
			}
		}
		//Update rat collision box
		m_rat_box.on_update(object()->position() - glm::vec3(0.f, object()->offset().y, 0.f) * object()->scale(), object()->rotation_amount(), object()->rotation_axis());
	}

}
//Sets upwards velocity towards the player to cause attack collision
void rat::attack_player(const engine::timestep& time_step, const glm::vec3& player_position) {
	if (attackDelay <= 0.0f) {
		m_speed = 0.5f;
		m_object->set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));
		m_object->set_forward(glm::vec3((player_position.x - m_object->position().x), 6.0f, (player_position.z - m_object->position().z)));
		m_object->set_position(m_object->position() + m_object->forward() * m_speed * (float)time_step);
		m_object->set_rotation_amount(atan2(-m_object->forward().x, -m_object->forward().z));
	}
	else {
		attackDelay -= 0.8f * time_step;
	}
}
void rat::flee_player(const engine::timestep& time_step) {
	isCurious = false;
	if (isFleeing == false) {
		//Find the nearest flee point
		float distPoint1 = glm::length(fleePoint1 - m_object->position());
		float distPoint2 = glm::length(fleePoint2 - m_object->position());
		float distPoint3 = glm::length(fleePoint3 - m_object->position());
		float distPoint4 = glm::length(fleePoint4 - m_object->position());

		std::vector<float> distances;
		distances.push_back(distPoint1);
		distances.push_back(distPoint2);
		distances.push_back(distPoint3);
		distances.push_back(distPoint4);

		int smallest = std::min_element(distances.begin(), distances.end()) - distances.begin();

		//Set flee goal to the nearest flee point
		if (smallest == 0) {
			fleeGoal = fleePoint1;
		}
		else if (smallest == 1) {
			fleeGoal = fleePoint2;
		}
		else if (smallest == 2) {
			fleeGoal = fleePoint3;
		}
		else {
			fleeGoal = fleePoint4;
		}
		isFleeing = true;
	}
	else {
		//Move towards the flee goal
		m_speed = 0.5f;
		m_object->set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));
		m_object->set_forward(fleeGoal - m_object->position());
		m_object->set_position(m_object->position() + m_object->forward() * m_speed * (float)time_step);
		m_object->set_rotation_amount(atan2(-m_object->forward().x, -m_object->forward().z));

		if (glm::length(fleeGoal - m_object->position()) < 2.0f) {
			toRemove = true;
		}
	}

}
//Picks a random coordinate in the similar area
void rat::search_player(const engine::timestep& time_step, const glm::vec3& player_position) {
	m_speed = 0.1f;
	if (headedToSearch == false) {
		//Creates position variable with y set to 1
		glm::vec3 thePosition = glm::vec3(0.f, 1.f, 0.f);
		//If in section 0
		if (((-12.8f <= m_object->position().z) && (m_object->position().z <= 29.0f)) && ((20.8f <= m_object->position().x) && (m_object->position().x <= 36.f))) {
			//Generate random float between x coordinates
			int rand1 = rand() % (3834 - 1870);
			float generateX = (float)rand1;
			generateX = (generateX / 100) + 18.70;
			//Generate random float between z coordinates
			int rand2 = rand() % (2900 + 1288);
			float generateZ = (float)rand2;
			generateZ = (generateZ / 100) - 12.88;
			//Saves the new coordinates
			thePosition.x = generateX;
			thePosition.z = generateZ;
		}
		//If in section 1
		else {
			//Generate random float between x coordinates
			int rand1 = rand() % (2500);
			float generateZ = (float)rand1;
			generateZ = (generateZ / 100) - 37.0;
			//Generate random float between z coordinates
			int rand2 = rand() % (7070);
			float generateX = (float)rand2;
			generateX = (generateX / 100) - 33.86;
			//Saves the new coordinates
			thePosition.x = generateX;
			thePosition.z = generateZ;
		}
		placeToSearch = thePosition;
		headedToSearch = true;
	}

	//Moves rat towards the search goal
	else if (headedToSearch == true) {
		m_object->set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));
		m_object->set_forward(placeToSearch - m_object->position());
		m_object->set_position(m_object->position() + m_object->forward() * m_speed * (float)time_step);
		m_object->set_rotation_amount(atan2(-m_object->forward().x, -m_object->forward().z));

		if (((placeToSearch.x - m_object->position().x) <= 3.0f) && ((placeToSearch.z - m_object->position().z) <= 3.0f)) {
			headedToSearch = false;
		}
	}

}
//Moves rat towards player position
void rat::chase_player(const engine::timestep& time_step, const glm::vec3& player_position) {
	m_speed = 0.8f;
	m_object->set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));
	m_object->set_forward(player_position - m_object->position());
	m_object->set_position(m_object->position() + m_object->forward() * m_speed * (float)time_step);
	m_object->set_rotation_amount(atan2(-m_object->forward().x, -m_object->forward().z));
}
//Moves rat towards curious location
void rat::curious(const engine::timestep& time_step) {
	m_speed = 0.1f;
	m_object->set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));
	m_object->set_forward(curiousGoal - m_object->position());
	m_object->set_position(m_object->position() + m_object->forward() * m_speed * (float)time_step);
	m_object->set_rotation_amount(atan2(-m_object->forward().x, -m_object->forward().z));
}

