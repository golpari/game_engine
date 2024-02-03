#ifndef ACTOR_H
#define ACTOR_H

#include <string>

#include "glm/glm.hpp"

class Actor
{
public:
	std::string actor_name;
	int actorID;
	glm::ivec2 position;
	glm::ivec2 velocity;
	char view;
	bool blocking;
	std::string nearby_dialogue;
	std::string contact_dialogue;
	bool scoredUpped;

	Actor(std::string actor_name_in, char view_in, glm::ivec2 position_in, glm::ivec2 initial_velocity_in,
		bool blocking_in, std::string nearby_dialogue_in, std::string contact_dialogue_in);

	Actor();
};
#endif