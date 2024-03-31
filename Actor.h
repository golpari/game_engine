#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <unordered_map>
#include <map>
#include "lua.hpp"
#include "LuaBridge.h"

extern int guuid;

class Actor
{
public:
	// keep actor_name, actorID, and template
	std::string actor_name;
	int actorID;
	std::map<std::string, std::shared_ptr<luabridge::LuaRef>> components; // local ID mapped to component (like the exact custom component for that actor)

	Actor(std::string actor_name_in)
		: actor_name(std::move(actor_name_in)), // Use std::move for strings
		actorID(guuid++) // Directly initialize to 0
	{}
};
#endif