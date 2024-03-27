#include "Scene.h"

void Scene::ProcessActors(rapidjson::Document& doc)
{
	std::string name = "";

	if (doc.HasMember("actors") && doc["actors"].IsArray()) {
		actors.reserve(doc["actors"].GetArray().Size());
		for (const auto& actor : doc["actors"].GetArray()) {
			//reset defaults
			name = "";

			//PROCESS EACH ACTOR
			if (actor.HasMember("template")) {
				//check if the template has already been processessed, if not then load it
				if (templates.find(actor["template"].GetString()) == templates.end()) {
					EngineUtils::ProcessTemplate(actor["template"].GetString());
				}
				ActorTemplate* actorTemplate = templates.at(actor["template"].GetString());
				
				// make the initial values inherit the template values
				name = actorTemplate->name;
			}

			// make the actor overwrite template values as needed 
			if (actor.HasMember("name")) { name = actor["name"].GetString(); }

			glm::vec2 position{ 0, 0 }; //artificial TODO GET RID OF THIS
			Actor* new_actor(new Actor(name));
			actors.push_back(new_actor);

			//instead of pushing back to the actors vector, push to optimized actors map
			addActorToMap(position, new_actor);
		} 
	}
}

void Scene::RenderScene()
{
	/*int x_res, y_res;
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
				//std::cout << actors_map.at(EngineUtils::combine(x, y)).back()->view;
			}
			else {
				std::cout << " ";
			}
		}
		std::cout << '\n';
	}*/
}


// PRIVATE HELPER FUNCTIONS
void Scene::addActorToMap(glm::vec2& position, Actor* new_actor) {
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

void Scene::updateActorPosition(Actor* actor, glm::vec2 newPos) {
	
	addActorToMap(newPos, actor);
	std::sort(actors_map[newPos].begin(), actors_map[newPos].end(), ActorComparator());
}

glm::vec2 Scene::getNewPosFromVelocity(glm::vec2& position, glm::vec2& velocity) {
	
	double x = position.x;
	double y = position.y;

	// Update x and y components with velocity
	x += velocity.x;
	y += velocity.y;
	glm::vec2 newPosition{ x, y };
	
	// return old position if actor wont go anywhere
	return position;
}
