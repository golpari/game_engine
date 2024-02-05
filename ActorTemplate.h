#ifndef ACTORTEMPLATE_H
#define ACTORTEMPLATE_H

#include <string>

#include "glm/glm.hpp"

class ActorTemplate
{
public:
	std::string name = "";
	int pos_x = 0;
	int pos_y = 0;
	int vel_y = 0;
	int vel_x = 0;
	char view = '?';
	bool blocking = false;
	std::string nearby_dialogue = "";
	std::string contact_dialogue = "";
	bool scoredUpped = false;

	ActorTemplate(std::string actor_name_in, char view_in, int pos_x_in, int pos_y_in, int vel_x_in, int vel_y_in,
		bool blocking_in, std::string nearby_dialogue_in, std::string contact_dialogue_in)
		: name(std::move(actor_name_in)), // Use std::move for strings
		pos_x(pos_x_in), 
		pos_y(pos_y_in),
		vel_x(vel_x_in),
		vel_y(vel_y_in),
		view(view_in),
		blocking(blocking_in),
		nearby_dialogue(std::move(nearby_dialogue_in)), // Use std::move for strings
		contact_dialogue(std::move(contact_dialogue_in)), // Use std::move for strings
		scoredUpped(false) // Directly initialize to false
	{}


	ActorTemplate() {}
};
#endif