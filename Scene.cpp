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

			//TEMPLATING
			if (actor.HasMember("template")) {
				//check if the template has already been processessed, if not then load it
				if (templates.find(actor["template"].GetString()) == templates.end()) {
					EngineUtils::ProcessTemplate(actor["template"].GetString());
				}
				ActorTemplate* actorTemplate = templates.at(actor["template"].GetString());

				// make the initial values inherit the template values
				name = actorTemplate->name;
			}
			if (actor.HasMember("name")) { name = actor["name"].GetString(); }

			Actor* new_actor(new Actor(name));

			/*When an actor is created…

				Queue it to have its components’ “OnStart” lifecycle function called at start of next frame.
				Components may or may not have an OnStart() lifecycle function.
				Components will only attempt to run their OnStart() function once upon their creation.
				Components should be processed in the alphabetical order of their key.
				Note : at game - start, the initial scene will load before the first frame.This means that Actors / components in the initial scene will have their OnStart() called on frame 0 (“the next frame coming”), rather than frame 1.
				Remember to pass in your ref as an argument to OnStart(and all other lifecycle functions) as seen here*/

			// Read through its json to find associated components.Add these components to the actor.
			if (actor.HasMember("components") && actor["components"].IsArray()) {
				for (const auto& component : actor["components"].GetArray()) {
					if (component.HasMember("type")) { 
						std::string componentType = component["type"].GetString();

						//make sure the component exists as a lua file
						if (!EngineUtils::CheckPathExists("resources/component_types/" + componentType + ".lua", false)) {
							std::cout << "error: failed to locate component " << componentType;
							exit(0);
						}

						// Establish inheritance using the raw lua api
						std::shared_ptr<luabridge::LuaRef> newTable = luabridge::newTable(lua_state);
						ComponentManager::EstablishInheritance(*newTable, *component_tables[componentType]); //the getGlobal is the parent Table

						new_actor->components[component.GetString()] = newTable;

						if ((*newTable).isTable()) {
							// check that there is an onStart function worth running
							luabridge::LuaRef onStart = (*newTable)["OnStart"];
							if (onStart.isFunction()) {
								onStarts.push_back(newTable); // push the component that needs to have onStart called to the onStarts vector in the scene class
							}
						}
					}
				}
			}

			

			glm::vec2 position{ 0, 0 }; //artificial TODO GET RID OF THIS
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
