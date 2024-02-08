#include "Scene.h"

bool Scene::CheckBlocking(uint64_t& position)
{
	if (actors_map.find(position) != actors_map.end()) {
		for (Actor* actor : actors_map.at(position)) {
			if (actor->blocking)
				return true;
		}
	}
	return false;
}

void Scene::ProcessActors(rapidjson::Document& doc)
{
	std::string name = "";
	int x = 0;
	int y = 0;
	int vel_x = 0;
	int vel_y = 0;
	char view = '?';
	bool blocking = false;
	std::string nearby_dialogue = "";
	std::string contact_dialogue = "";

	if (doc.HasMember("actors") && doc["actors"].IsArray()) {
		actors.reserve(doc["actors"].GetArray().Size());
		for (const auto& actor : doc["actors"].GetArray()) {
			//reset defaults
			name = "";
			x = 0;
			y = 0;
			vel_x = 0;
			vel_y = 0;
			view = '?';
			blocking = false;
			nearby_dialogue = "";
			contact_dialogue = "";

			//PROCESS EACH ACTOR
			if (actor.HasMember("template")) {
				//check if the template has already been processessed, if not then load it
				if (templates.find(actor["template"].GetString()) == templates.end()) {
					EngineUtils::ProcessTemplate(actor["template"].GetString());
				}
				ActorTemplate* actorTemplate = templates.at(actor["template"].GetString());

				// make the initial values inherit the template values
				name = actorTemplate->name;
				x = actorTemplate->pos_x;
				y = actorTemplate->pos_y;
				vel_x = actorTemplate->vel_x;
				vel_y = actorTemplate->vel_y;
				view = actorTemplate->view;
				blocking = actorTemplate->blocking;
				nearby_dialogue = actorTemplate->nearby_dialogue;
				contact_dialogue = actorTemplate->contact_dialogue;
			}

			// make the actor overwrite template values as needed 
			if (actor.HasMember("name")) { name = actor["name"].GetString(); }
			if (actor.HasMember("x")) { x = actor["x"].GetInt(); }
			if (actor.HasMember("y")) { y = actor["y"].GetInt(); }
			if (actor.HasMember("vel_x")) { vel_x = actor["vel_x"].GetInt(); }
			if (actor.HasMember("vel_y")) { vel_y = actor["vel_y"].GetInt(); }
			if (actor.HasMember("view")) { view = *actor["view"].GetString(); }
			if (actor.HasMember("blocking")) { blocking = actor["blocking"].GetBool(); }
			if (actor.HasMember("nearby_dialogue")) { nearby_dialogue = actor["nearby_dialogue"].GetString(); }
			if (actor.HasMember("contact_dialogue")) { contact_dialogue = actor["contact_dialogue"].GetString(); }
			
			// create actor variable and store it in list of actors
			uint64_t position = EngineUtils::combine(x, y);
			glm::ivec2 velocity{ vel_x, vel_y };
			Actor* new_actor(new Actor(name, view, position, velocity, blocking, nearby_dialogue, contact_dialogue));
			actors.push_back(new_actor);

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
	//if not valid movement, do nothing
	if (movement != "n" &&
		movement != "e" &&
		movement != "s" &&
		movement != "w")
		return;

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
	if (CheckBlocking(tempPosition))  // Don't do anything if blocking wall is there
		return;

	// Update player's position if not blocked
	updateActorPosition(player, tempPosition);
}

void Scene::MoveActors() {
	//update all actors except for the player (which is the last actor)
	for (int i = 0; i < actors.size(); i++) {
		if (actors[i]->actor_name != "player") {
			updateActorPosition(actors[i], getNewPosFromVelocity(actors[i]->position, actors[i]->velocity));
		}
	}
}
/*
void Scene::RenderScene()
{
	int x_res, y_res;
	EngineUtils::split(Game::GetCameraResolution(), x_res, y_res);

	// Calculate starting and ending coordinates for the customizable grid
	int posX, posY;
	EngineUtils::split(player->position, posX, posY);
	int startX = posX - (x_res - 1) / 2;
	int endX = posX + (x_res - 1) / 2;

	int startY = posY - (y_res - 1) / 2;
	int endY = posY + (y_res - 1) / 2;

	bool actorPresent = false;

	for (int y = startY; y <= endY; ++y) {
		for (int x = startX; x <= endX; ++x) {
			//check the map and see if there is an actor present
			if (actors_map.find(EngineUtils::combine(x, y)) != actors_map.end()) {
				// use .back() to render highest actorID on top (aka the actor loaded in last)
				std::cout << actors_map.at(EngineUtils::combine(x, y)).back()->view;
			}
			else {
				std::cout << " ";
			}
		}
		std::cout << '\n';
	}
}
*/

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
	const uint64_t oldPos = actor->position;
	// get actors position vector
	std::vector<Actor*>& oldVec = actors_map[oldPos];

	// delete the actor from the position vector
	oldVec.erase(std::remove(oldVec.begin(), oldVec.end(), actor), oldVec.end());

	// delete the position vector / kv pair if its empty
	if (oldVec.empty()) {
		actors_map.erase(oldPos);
	}

	// Update the Actor's position property
	actor->position = newPos;

	// Add actor to new position vector
	addActorToMap(newPos, actor);
	std::sort(actors_map[newPos].begin(), actors_map[newPos].end(), ActorComparator());
}

uint64_t Scene::getNewPosFromVelocity(uint64_t& position, glm::ivec2& velocity) {
	
	int x, y = 0;
	EngineUtils::split(position, x, y);

	// Update x and y components with velocity
	x += velocity.x;
	y += velocity.y;
	uint64_t newPosition = EngineUtils::combine(x, y);

	//only return the updated position if its not blocked
	if (!CheckBlocking(newPosition)) {
		return newPosition;
		// IF ITS BLOCKED FLIP THE VELOCITY !!!!
	}
	else {
		velocity = -velocity;
	}
	
	// return old position if actor wont go anywhere
	return position;
}
