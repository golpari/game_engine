#ifndef ACTOR_H
#define ACTOR_H

#include <string>

#include "glm/glm.hpp"

extern int guuid;

class Actor
{
public:
	// TODO: comment this out
	std::string actor_name;
	int actorID;
	uint64_t position;
	glm::ivec2 velocity;
	char view;
	bool blocking;
	std::string nearby_dialogue;
	std::string contact_dialogue;
	bool scoredUpped;

	std::string view_image = "";
	glm::vec2 scale = { 1.0, 1.0 }; // TODO: use std::abs() when importing new scales
	// TODO: negative scale shld cause actor to 'flip' -- use SDL_RendererFlip
	double rotation = 0.0;
	glm::vec2 pivot_offset; // default is actor_view.w * 0.5 (double) and actor_view.h * 0.5 (double)
	// TODO: use std::round() to calculate pivot. Pivot point is the top left of image PLUS offset

	Actor(std::string actor_name_in, char view_in, uint64_t position_in, glm::ivec2 initial_velocity_in,
		bool blocking_in, std::string nearby_dialogue_in, std::string contact_dialogue_in)
		: actor_name(std::move(actor_name_in)), // Use std::move for strings
		actorID(guuid++), // Directly initialize to 0
		position(std::move(position_in)), // Use std::move if glm::ivec2 has a move constructor; otherwise, it's fine as is
		velocity(std::move(initial_velocity_in)), // Same as position
		view(view_in),
		blocking(blocking_in),
		nearby_dialogue(std::move(nearby_dialogue_in)), // Use std::move for strings
		contact_dialogue(std::move(contact_dialogue_in)), // Use std::move for strings
		scoredUpped(false) // Directly initialize to false
	{}


	Actor() {
		actor_name = "";
		actorID = 0;
		position = 00;
		velocity = { 0,0 };
		view = '?';
		blocking = false;
		nearby_dialogue = "";
		contact_dialogue = "";
		scoredUpped = false;
	};
};
#endif