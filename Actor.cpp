#include "Actor.h"

Actor::Actor(std::string actor_name_in, char view_in, glm::ivec2 position_in, glm::ivec2 initial_velocity_in,
	bool blocking_in, std::string nearby_dialogue_in, std::string contact_dialogue_in) {
	actor_name = actor_name_in;
	actorID = 0;
	position = position_in;
	velocity = initial_velocity_in;
	view = view_in;
	blocking = blocking_in;
	nearby_dialogue = nearby_dialogue_in;
	contact_dialogue = contact_dialogue_in;
	scoredUpped = false;
}
