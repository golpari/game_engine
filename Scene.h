#ifndef SCENE_H
#define SCENE_H

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Actor.h"
#include "EngineUtils.h"
#include "Game.h"
#include "glm/glm.hpp"
#include "rapidjson/document.h"

extern std::unordered_map<std::string, ActorTemplate*> templates;

class Scene
{
public:

	std::unordered_map<uint64_t, std::vector<Actor*>> actors_map;
	std::vector<Actor*> actors;
	Actor* player;

	Scene() { player = nullptr; }

	bool CheckBlocking(uint64_t& position);

	void ProcessActors(rapidjson::Document& doc);

	void MovePlayer(std::string& movement);

	void MoveActors();

	void RenderScene();

private:
	// chat help
	void addActorToMap(uint64_t& position, Actor* new_actor);

	// chat help
	void updateActorPosition(Actor* actor, uint64_t newPos);

	// chat help
	uint64_t getNewPosFromVelocity(uint64_t& position, glm::ivec2& velocity);
};
#endif

