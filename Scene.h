#ifndef SCENE_H
#define SCENE_H

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <optional>

#include "Actor.h"
#include "EngineUtils.h"
#include "Game.h"
#include "ComponentManager.h"
#include "glm/glm.hpp"
#include "rapidjson/document.h"
#include "lua.hpp"
#include "LuaBridge.h"

extern std::unordered_map<std::string, ActorTemplate*> templates;
extern lua_State* lua_state;
extern std::unordered_map<std::string, std::shared_ptr<luabridge::LuaRef>> component_tables; //map id to component type

class hashFunction {
public:

	// Use sum of lengths of x and y positions
	// as hash function.
	size_t operator()(const glm::vec2& position) const
	{
		return EngineUtils::combine(position.x, position.y);
	}
};

class Scene
{
public:

	std::unordered_map<glm::vec2, std::vector<Actor*>, hashFunction> actors_map;
	std::vector<Actor*> actors;
	std::vector<std::shared_ptr<luabridge::LuaRef>> onStarts;

	Scene() {}

	void ProcessActors(rapidjson::Document& doc);

	void RenderScene();

private:
	// chat help
	void addActorToMap(glm::vec2& position, Actor* new_actor);

	// chat help
	void updateActorPosition(Actor* actor, glm::vec2 newPos);

	// chat help
	glm::vec2 getNewPosFromVelocity(glm::vec2& position, glm::vec2& velocity);
};

#endif

