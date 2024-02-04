#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

#include "Actor.h"
#include "EngineUtils.h"
#include "Game.h"
#include "glm/glm.hpp"
#include "rapidjson/document.h"

class Scene
{
public:
	std::unordered_map<uint64_t, std::vector<Actor*>> actors_map;

	std::vector<Actor*> actors;

	Actor* player;

	bool CheckBlocking(uint64_t& position);

	void ProcessActors(rapidjson::Document& doc);

	void MovePlayer(std::string& movement);

	void MoveActors();

	void RenderScene();

private:
	void addActorToMap(uint64_t& position, Actor* new_actor);

	void updateActorPosition(Actor* actor, uint64_t newPos);

	uint64_t getNewPosFromVelocity(uint64_t& position, const glm::ivec2& velocity);
};
#endif

