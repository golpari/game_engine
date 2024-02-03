#ifndef ACTOR_H
#define ACTOR_H

#include <string>

#include "glm/glm.hpp"

class Actor
{
public:
	std::string actor_name;
	int actorID;
	uint64_t position;
	glm::ivec2 velocity;
	char view;
	bool blocking;
	std::string nearby_dialogue;
	std::string contact_dialogue;
	bool scoredUpped;

	Actor(std::string actor_name_in, char view_in, uint64_t position_in, glm::ivec2 initial_velocity_in,
		bool blocking_in, std::string nearby_dialogue_in, std::string contact_dialogue_in)
		: actor_name(std::move(actor_name_in)), // Use std::move for strings
		actorID(0), // Directly initialize to 0
		position(std::move(position_in)), // Use std::move if glm::ivec2 has a move constructor; otherwise, it's fine as is
		velocity(std::move(initial_velocity_in)), // Same as position
		view(view_in),
		blocking(blocking_in),
		nearby_dialogue(std::move(nearby_dialogue_in)), // Use std::move for strings
		contact_dialogue(std::move(contact_dialogue_in)), // Use std::move for strings
		scoredUpped(false) // Directly initialize to false
	{}


	Actor();
};
#endif