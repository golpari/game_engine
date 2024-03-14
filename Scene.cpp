#include "Scene.h"

bool Scene::CheckBlocking(glm::vec2& position)
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
	double x = 0;
	double y = 0;
	double vel_x = 0;
	double vel_y = 0;
	char view = '?';
	bool blocking = false;
	std::string nearby_dialogue = "";
	std::string contact_dialogue = "";

	std::string view_image = "";
	double scaleX = 1.0;
	double scaleY = 1.0;
	double rotation_deg = 0.0;

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

			view_image = "";
			scaleX = 1.0;
			scaleY = 1.0;
			rotation_deg = 0.0;
			std::optional<double> pivot_offsetX; // actual default is actor_view.w * 0.5
			std::optional<double> pivot_offsetY; // actual default is actor_view.h * 0.5
			std::optional<double> render_order; //default is transform_y if render_order not set

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

				view_image = actorTemplate->view_image;
				scaleX = actorTemplate->scaleX;
				scaleY = actorTemplate->scaleY;
				rotation_deg = actorTemplate->rotation;
				pivot_offsetX = actorTemplate->pivot_offsetX;
				pivot_offsetY = actorTemplate->pivot_offsetY;
				render_order = actorTemplate->render_order;
			}

			// make the actor overwrite template values as needed 
			if (actor.HasMember("name")) { name = actor["name"].GetString(); }
			if (actor.HasMember("transform_position_x")) { x = actor["transform_position_x"].GetDouble(); }
			if (actor.HasMember("transform_position_y")) { y = actor["transform_position_y"].GetDouble(); }
			if (actor.HasMember("vel_x")) { vel_x = actor["vel_x"].GetDouble(); }
			if (actor.HasMember("vel_y")) { vel_y = actor["vel_y"].GetDouble(); }
			if (actor.HasMember("view")) { view = *actor["view"].GetString(); }
			if (actor.HasMember("blocking")) { blocking = actor["blocking"].GetBool(); }
			if (actor.HasMember("nearby_dialogue")) { nearby_dialogue = actor["nearby_dialogue"].GetString(); }
			if (actor.HasMember("contact_dialogue")) { contact_dialogue = actor["contact_dialogue"].GetString(); }

			if (actor.HasMember("view_image")) { view_image = actor["view_image"].GetString(); }
			if (actor.HasMember("transform_scale_x")) { scaleX = actor["transform_scale_x"].GetDouble(); }
			if (actor.HasMember("transform_scale_y")) { scaleY = actor["transform_scale_y"].GetDouble(); }
			if (actor.HasMember("transform_rotation_degrees")) { rotation_deg = actor["transform_rotation_degrees"].GetDouble(); }
			if (actor.HasMember("view_pivot_offset_x")) { pivot_offsetX = actor["view_pivot_offset_x"].GetDouble(); }
			if (actor.HasMember("view_pivot_offset_y")) { pivot_offsetY = actor["view_pivot_offset_y"].GetDouble(); }
			if (actor.HasMember("render_order")) { render_order = actor["render_order"].GetDouble(); }
			
			// create actor variable and store it in list of actors
			glm::vec2 position{ x, y };
			glm::ivec2 velocity{ vel_x, vel_y };
			glm::vec2 scale{ scaleX, scaleY };

			//glm::vec2 pivot_offset{ pivot_offsetX, pivot_offsetY };
			Actor* new_actor(new Actor(name, /*view, */position, velocity, blocking, nearby_dialogue, contact_dialogue,
				view_image, scale, rotation_deg, pivot_offsetX, pivot_offsetY, render_order));
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

void Scene::MovePlayer(glm::vec2& direction, double speed)
{	
	glm::vec2 tempPosition;

	// normalize the vector if diagonal
	if (glm::length(direction) > 0.001f) tempPosition = player->position + ((glm::normalize(direction) * static_cast<float>(speed)));
	else tempPosition = player->position + (direction * static_cast<float>(speed));

	// Check for blocking at the new position
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
	const glm::vec2 oldPos = actor->position;
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

glm::vec2 Scene::getNewPosFromVelocity(glm::vec2& position, glm::vec2& velocity) {
	
	double x = position.x;
	double y = position.y;

	// Update x and y components with velocity
	x += velocity.x;
	y += velocity.y;
	glm::vec2 newPosition{ x, y };

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
