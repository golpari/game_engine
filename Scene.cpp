#include "Scene.h"

bool Scene::CheckBlocking(glm::ivec2& position)
{
	//if (hardcoded_map[position.y][position.x] == 'b')
	//	return true;
	/*for (Actor actor : actors) {
		if (actor.position == position && actor.blocking)
			return true;
	}*/

	//check the map
	return false;
}

void Scene::ProcessActors(rapidjson::Document& doc)
{
	// Use the parsed document here...
	// For example, access the 'actors' array
	std::string name = "";
	int x = 0;
	int y = 0;
	int vel_x = 0;
	int vel_y = 0;
	char view = '?';
	bool blocking = false;
	std::string nearby_dialogue = "";
	std::string contact_dialogue = "";

	glm::ivec2 position;
	glm::ivec2 velocity;

	if (doc.HasMember("actors") && doc["actors"].IsArray()) {
		actors.reserve(doc["actors"].GetArray().Size());
		for (const auto& actor : doc["actors"].GetArray()) {
			//PROCESS EACH ACTOR
			if (actor.HasMember("name")) {
				name = actor["name"].GetString();
			}
			if (actor.HasMember("x")) {
				x = actor["x"].GetInt();
			}
			if (actor.HasMember("y")) {
				y = actor["y"].GetInt();
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
			uint64_t position = EngineUtils::combine(x, y);
			glm::ivec2 velocity{ vel_x, vel_y };
			Actor* new_actor = new Actor(name, view, position, velocity, blocking, nearby_dialogue, contact_dialogue);
			//actors.push_back(new_actor);

			//instead of pushing back to the actors vector, push to optimized actors map
			addActorToMap(position, new_actor);

			// store a pointer directly to the player
			if (name == "player") {
				player = new_actor;
			}
		} 
	}
}

void Scene::MovePlayer(std::string& movement)
{
	glm::ivec2 temp;
	//if not valid movement, do nothing
	if (movement != "n" &&
		movement != "e" &&
		movement != "s" &&
		movement != "w")
		return;

	/*update position vec
	else if (movement == "n") {
		temp = glm::ivec2{ player->position.x, player->position.y - 1 };
		if (CheckBlocking(temp)) //dont do anything if b (blocking) wall is there
			return;
		player->position.y--;
	}
	else if (movement == "e") {
		temp = glm::ivec2{ player->position.x + 1, player->position.y };
		if (CheckBlocking(temp)) //dont do anything if b (blocking) wall is there
			return;
		player->position.x++;
	}
	else if (movement == "s") {
		temp = glm::ivec2{ player->position.x, player->position.y + 1 };
		if (CheckBlocking(temp)) //dont do anything if b (blocking) wall is there
			return;
		player->position.y++;
	}
	else if (movement == "w") {
		temp = glm::ivec2{ player->position.x - 1, player->position.y };
		if (CheckBlocking(temp)) //dont do anything if b (blocking) wall is there
			return;
		player->position.x--;
	}*/

	int x, y;
	// Split the current position into x and y
	EngineUtils::split(player->position, x, y);

	// Temporary variables to hold new positions
	int newX = x;
	int newY = y;

	// Determine new position based on movement direction
	if (movement == "n") {
		newY = y - 1;
	}
	else if (movement == "e") {
		newX = x + 1;
	}
	else if (movement == "s") {
		newY = y + 1;
	}
	else if (movement == "w") {
		newX = x - 1;
	}

	// Check for blocking at the new position
	uint64_t tempPosition = EngineUtils::combine(newX, newY);
	glm::ivec2 position{ 0,0 };
	if (CheckBlocking(position))  // Don't do anything if blocking wall is there
		return;

	// Update player's position if not blocked
	updateActorPosition(player, EngineUtils::combine(newX, newY));
}

void Scene::MoveActors() {
	glm::ivec2 nextPosition;
	//update all actors except for the player (which is the last actor)
	/*for (int i = 0; i < actors.size() - 1; i++) {
		if (actors[i].actor_name != "player") {
			nextPosition = actors[i].position + actors[i].velocity;
			if (!CheckBlocking(nextPosition))
				actors[i].position = nextPosition;
			else
				actors[i].velocity = -actors[i].velocity;
		}
	}*/
}

void Scene::RenderScene()
{
	int x_res;
	int y_res;
	EngineUtils::split(Game::GetCameraResolution(), x_res, y_res);

	// Calculate starting and ending coordinates for the customizable grid
	int posX;
	int posY;
	EngineUtils::split(player->position, posX, posY);
	int startX = posX - (x_res - 1) / 2;
	int endX = posX + (x_res - 1) / 2;

	int startY = posY - (y_res - 1) / 2;
	int endY = posY + (y_res - 1) / 2;

	bool actorPresent = false;
	Actor* actorToPrint;

	for (int y = startY; y <= endY; ++y) {
		for (int x = startX; x <= endX; ++x) {
			//instead, just check the map and see if there is an actor present
			if (actors_map.find(EngineUtils::combine(x, y)) != actors_map.end()) {
				std::cout << actors_map.at(EngineUtils::combine(x, y)).front()->view;
			}
			/*actorPresent = false;

			// Check if there's an actor at the current position
			char viewToPrint = ' ';
			for (Actor actor : actors) {
				if (actor.position == glm::ivec2{ x, y }) {
					// if another actor isnt already gonna be rendered on top, then set the render view
					if (!actorPresent) {
						viewToPrint = actor.view;
					}
					actorPresent = true;
					
				}
			}
			//print a space whenever there is no actor being rendered, since now all items are stored as actors
			if(!actorPresent)
				std::cout << ' ';*/
			else {
				std::cout << " ";
			}
		}
		std::cout << '\n';
	}

	return;
}

// PRIVATE HELPER FUNCTIONS
void Scene::addActorToMap(uint64_t& position, Actor* new_actor) {
	// Check if the position key already exists in the map
	auto it = actors_map.find(position);
	if (it != actors_map.end()) {
		// Position key exists, add new_actor to the existing vector
		it->second.push_back(new_actor);
	}
	else {
		// Position key does not exist, create a new vector and add new_actor
		actors_map[position] = std::vector<Actor*>{ new_actor };
	}
}

void Scene::updateActorPosition(Actor* actor, uint64_t newPos) {
	uint64_t oldPos = actor->position;
	// Remove actor from old position vector
	auto& oldVec = actors_map[oldPos];
	oldVec.erase(std::remove(oldVec.begin(), oldVec.end(), actor), oldVec.end());
	if (oldVec.empty()) {
		actors_map.erase(oldPos);
	}

	// Add actor to new position vector
	actors_map[newPos].push_back(actor);
	// Update the Actor's position property
	actor->position = newPos;
}
