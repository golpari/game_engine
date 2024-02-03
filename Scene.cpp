#include "Scene.h"
#include "rapidjson/document.h"
#include "EngineUtils.h"
#include "glm/glm.hpp"
#include "Game.h"

bool Scene::CheckBlocking(glm::ivec2 position)
{
	//if (hardcoded_map[position.y][position.x] == 'b')
	//	return true;
	for (Actor actor : actors) {
		if (actor.position == position && actor.blocking)
			return true;
	}
	return false;
}

void Scene::ProcessActors(rapidjson::Document& doc)
{
	// Use the parsed document here...
	// For example, access the 'actors' array
	std::string name = "";
	uint32_t x = 0;
	uint32_t y = 0;
	uint32_t vel_x = 0;
	uint32_t vel_y = 0;
	char view = '?';
	bool blocking = false;
	std::string nearby_dialogue = "";
	std::string contact_dialogue = "";

	glm::ivec2 position;
	glm::ivec2 velocity;

	if (doc.HasMember("actors") && doc["actors"].IsArray()) {
		for (const auto& actor : doc["actors"].GetArray()) {
			//PROCESS EACH ACTOR
			if (actor.HasMember("name")) {
				name = actor["name"].GetString();
			}
			if (actor.HasMember("x")) {
				x = actor["x"].GetInt();
			}
			if (actor.HasMember("y")) {
				x = actor["y"].GetInt();
			}
			if (actor.HasMember("vel_x")) {
				vel_x = actor["vel_x"].GetInt();
			}
			if (actor.HasMember("vel_y")) {
				vel_y = actor["vel_y"].GetInt();
			}
			if (actor.HasMember("view")) {
				view = *actor["view"].GetString();
			}
			if (actor.HasMember("blocking")) {
				blocking = actor["blocking"].GetBool();
			}
			if (actor.HasMember("nearby_dialogue")) {
				nearby_dialogue = actor["nearby_dialogue"].GetString();
			}
			if (actor.HasMember("contact_dialogue")) {
				contact_dialogue = actor["contact_dialogue"].GetString();
			}
			
			// create actor variable and store it in list of actors
			glm::ivec2 position{ x, y };
			glm::ivec2 velocity{ vel_x, vel_y };
			Actor new_actor(name, view, position, velocity, blocking, nearby_dialogue, contact_dialogue);
			actors.push_back(new_actor);

			// store a pointer directly to the player
			if (name == "player") {
				player = &actors.back();
			}
		}
	}
}

void Scene::MovePlayer(std::string& movement)
{
	//if not valid movement, do nothing
	if (movement != "n" &&
		movement != "e" &&
		movement != "s" &&
		movement != "w")
		return;

	//update position vec
	else if (movement == "n") {
		//DEBUG STATEMENT std::cout << "HERE: " << hardcoded_map[actor.position.y - 1][actor.position.x] << std::endl;
		if (CheckBlocking(glm::ivec2(player->position.x, player->position.y - 1))) //dont do anything if b (blocking) wall is there
			return;
		player->position.y--;
	}
	else if (movement == "e") {
		if (CheckBlocking(glm::ivec2(player->position.x + 1, player->position.y))) //dont do anything if b (blocking) wall is there
			return;
		player->position.x++;
	}
	else if (movement == "s") {
		if (CheckBlocking(glm::ivec2(player->position.x, player->position.y + 1))) //dont do anything if b (blocking) wall is there
			return;
		player->position.y++;
	}
	else if (movement == "w") {
		if (CheckBlocking(glm::ivec2(player->position.x - 1, player->position.y))) //dont do anything if b (blocking) wall is there
			return;
		player->position.x--;
	}
}

void Scene::MoveActors() {
	glm::ivec2 nextPosition;
	//update all actors except for the player (which is the last actor)
	for (int i = 0; i < actors.size() - 1; i++) {
		if (actors[i].actor_name != "player") {
			nextPosition = actors[i].position + actors[i].velocity;
			if (!CheckBlocking(nextPosition))
				actors[i].position = nextPosition;
			else
				actors[i].velocity = -actors[i].velocity;
		}
	}
}

void Scene::RenderScene()
{
	uint32_t x_res;
	uint32_t y_res;
	EngineUtils::split(Game::GetCameraResolution(), x_res, y_res);

	// Calculate starting and ending coordinates for the customizable grid
	int startX = player->position.x - (x_res - 1) / 2;
	int endX = player->position.x + (x_res - 1) / 2;

	int startY = player->position.y - (y_res - 1) / 2;
	int endY = player->position.y + (y_res - 1) / 2;

	bool actorPresent = false;
	Actor* actorToPrint;

	for (int y = startY; y <= endY; ++y) {
		for (int x = startX; x <= endX; ++x) {
			actorPresent = false;

			// Check if there's an actor at the current position
			char viewToPrint = ' ';
			for (Actor actor : actors) {
				if (actor.position == glm::ivec2{ x, y }) {
					actorPresent = true;
					viewToPrint = actor.view;
				}
			}
			//print a space whenever there is no actor being rendered, since now all items are stored as actors
			if(!actorPresent)
				std::cout << ' ';
			else {
				std::cout << viewToPrint;
			}
		}
		std::cout << '\n';
	}

	return;
}
