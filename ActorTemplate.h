#ifndef ACTORTEMPLATE_H
#define ACTORTEMPLATE_H

#include <string>

class ActorTemplate
{
public:
	std::string name = "";

	ActorTemplate(std::string actor_name_in)
		: name(std::move(actor_name_in)) // Use std::move for strings
	{}


	ActorTemplate() {}
};
#endif