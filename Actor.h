#ifndef ACTOR_H
#define ACTOR_H

#include <string>

extern int guuid;

class Actor
{
public:
	// keep actor_name, actorID, and template
	std::string actor_name;
	int actorID;

	Actor(std::string actor_name_in)
		: actor_name(std::move(actor_name_in)), // Use std::move for strings
		actorID(guuid++) // Directly initialize to 0
	{}
};
#endif