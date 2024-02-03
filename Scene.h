#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include "rapidjson/document.h"

#include "Actor.h"
#include "glm/glm.hpp"

class Scene
{
public:
	std::vector<Actor> actors;

	Actor* player;

	bool CheckBlocking(glm::ivec2 position);

	void ProcessActors(rapidjson::Document& doc);

	void MovePlayer(std::string& movement);

	void MoveActors();

	void RenderScene();
};
#endif

