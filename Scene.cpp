#include "Scene.h"

bool Scene::CheckBlocking(glm::vec2& position)
{
	// Roughly how the autograder deals with homework6 collision-related happenings.
	// A key thing to note-- dialogue-processing occurs after all actor updates.
	/*
	1) Begin iterating through actors, calling Update() on each one.
		1.1) Determine the "desired new position" of the actor(logic different if player vs npc).
		1.2) If the actor has a valid box collider, collect a vector of all other actors colliding with it at "desired new position" (AABB).
		1.2.1) For each of these actors we collide with at "desired new position", add them to our "colliding_actors_this_frame" set...
		1.2.2) ...and add ourselves to the other actor's set.
		1.3) If the colliding_actors_this_frame set has a size larger than 0 (IE, we collided with something)...
		1.3.1) Do not move this frame.
		1.3.2) Invert our velocity(only meaningful for NPCs).
		1.4) If the colliding_actors_this_frame set has a size of 0, move this actor to "desired new position".


		2) Collect and render dialogue(and process dialogue commands). // All actors have been updated at this point.
		2.1) Find any non - player actors in the player's region (or neighboring regions).
		2.2) Perform an AABB test on these "nearby" actors to collect actors whos triggers are overlapping the player trigger.
		2.3) Process and render the nearby_dialogue of these actors.
		2.4) Process the contact_dialogue of any actors that collided(box colliders) with the player earlier during the frame. */

	/*if (actors_map.find(position) != actors_map.end()) {
		for (Actor* actor : actors_map.at(position)) {
			if (actor->blocking)
				return true;
		}
	}*/
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
	float boxCollWidth = 0;
	float boxCollHeight = 0;
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
			boxCollWidth = 0;
			boxCollHeight = 0;
			nearby_dialogue = "";
			contact_dialogue = "";

			view_image = "";
			scaleX = 1.0;
			scaleY = 1.0;
			rotation_deg = 0.0;
			std::optional<double> pivot_offsetX; // actual default is actor_view.w * 0.5
			std::optional<double> pivot_offsetY; // actual default is actor_view.h * 0.5
			std::optional<double> render_order; //default is transform_y if render_order not set
			std::optional<std::string> view_image_back;
			bool movementBounce = false;

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
				//blocking = actorTemplate->blocking;
				// TODO: ADD BOXCOLL TEMPLATE PROCESSING
				nearby_dialogue = actorTemplate->nearby_dialogue;
				contact_dialogue = actorTemplate->contact_dialogue;

				view_image = actorTemplate->view_image;
				scaleX = actorTemplate->scaleX;
				scaleY = actorTemplate->scaleY;
				rotation_deg = actorTemplate->rotation;
				pivot_offsetX = actorTemplate->pivot_offsetX;
				pivot_offsetY = actorTemplate->pivot_offsetY;
				render_order = actorTemplate->render_order;
				view_image_back = actorTemplate->view_image_back;
				movementBounce = actorTemplate->movementBounce;
			}

			// make the actor overwrite template values as needed 
			if (actor.HasMember("name")) { name = actor["name"].GetString(); }
			if (actor.HasMember("transform_position_x")) { x = actor["transform_position_x"].GetDouble(); }
			if (actor.HasMember("transform_position_y")) { y = actor["transform_position_y"].GetDouble(); }
			if (actor.HasMember("vel_x")) { vel_x = actor["vel_x"].GetDouble(); }
			if (actor.HasMember("vel_y")) { vel_y = actor["vel_y"].GetDouble(); }
			if (actor.HasMember("view")) { view = *actor["view"].GetString(); }
			if (actor.HasMember("box_collider_width")) { boxCollWidth = actor["box_collider_width"].GetFloat(); }
			if (actor.HasMember("box_collider_height")) { boxCollHeight = actor["box_collider_height"].GetFloat(); }
			if (actor.HasMember("nearby_dialogue")) { nearby_dialogue = actor["nearby_dialogue"].GetString(); }
			if (actor.HasMember("contact_dialogue")) { contact_dialogue = actor["contact_dialogue"].GetString(); }

			if (actor.HasMember("view_image")) { view_image = actor["view_image"].GetString(); }
			if (actor.HasMember("transform_scale_x")) { scaleX = actor["transform_scale_x"].GetDouble(); }
			if (actor.HasMember("transform_scale_y")) { scaleY = actor["transform_scale_y"].GetDouble(); }
			if (actor.HasMember("transform_rotation_degrees")) { rotation_deg = actor["transform_rotation_degrees"].GetDouble(); }
			if (actor.HasMember("view_pivot_offset_x")) { pivot_offsetX = actor["view_pivot_offset_x"].GetDouble(); }
			if (actor.HasMember("view_pivot_offset_y")) { pivot_offsetY = actor["view_pivot_offset_y"].GetDouble(); }
			if (actor.HasMember("render_order")) { render_order = actor["render_order"].GetDouble(); }
			if (actor.HasMember("view_image_back")) { view_image_back = actor["view_image_back"].GetString(); }
			if (actor.HasMember("movement_bounce_enabled")) { movementBounce = actor["movement_bounce_enabled"].GetBool(); }
			
			// create actor variable and store it in list of actors
			glm::vec2 position{ x, y };
			glm::vec2 velocity{ vel_x, vel_y };
			glm::vec2 scale{ scaleX, scaleY };

			// create the box collider, the collider needs to get updated every time the actor moves
			BoxCollider collider;
			collider.top = position.y;
			collider.bottom = position.y + boxCollHeight;
			collider.left = position.x;
			collider.right = position.x + boxCollWidth;

			//glm::vec2 pivot_offset{ pivot_offsetX, pivot_offsetY };
			Actor* new_actor(new Actor(name, /*view, */position, velocity, collider, nearby_dialogue, contact_dialogue,
				view_image, scale, rotation_deg, pivot_offsetX, pivot_offsetY, render_order, view_image_back, movementBounce));
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

void Scene::MovePlayer(glm::vec2& direction, double speed, bool flip)
{	
	glm::vec2 tempPosition;

	// normalize the vector if diagonal
	if (glm::length(direction) > 0.001f) 
		tempPosition = player->position + ((glm::normalize(direction) * static_cast<float>(speed)));

	else tempPosition = player->position + (direction * static_cast<float>(speed));

	// Check for blocking at the new position
	if (CheckBlocking(tempPosition))  // Don't do anything if blocking wall is there
		return;

	// Update player's position if not blocked
	updateActorPosition(player, tempPosition);
	AnimatePlayer(direction, flip);

}

void Scene::MoveActors(bool flip) {
	//update all actors except for the player (which is the last actor)
	//1) Begin iterating through actors, calling Update() on each one.
	for (int i = 0; i < actors.size(); i++) {
		if (actors[i]->actor_name != "player") {
			//1.1) Determine the "desired new position" of the actor(logic different if player vs npc).
			glm::vec2 newPos = getNewPosFromVelocity(actors[i]->position, actors[i]->velocity);
// TODO do i need to make the box1 collider be based on the new position and to also update the box1 collider?
			//1.2) If the actor has a valid box collider, collect a vector of all other actors colliding with it at "desired new position" (AABB).
			if (actors[i]->collider.left < actors[i]->collider.right && actors[i]->collider.top < actors[i]->collider.bottom) {
				// AABB COLLISION
				/*if (box1.left < box2.right
					&& box1.right > box2.left
					&& box1.top < box2.bottom
					&& box1.bottom > box2.top)
				{
					// boxes 1 and 2 are colliding
				}*/
				std::vector<Actor*> collidingActors;
				for (int j = 0; j < actors.size(); j++) {
					// make sure not to check for a collision against yourself
					if (actors[j] != actors[i]) {
						if (actors[i]->collider.left < actors[j]->collider.right
							&& actors[i]->collider.right > actors[j]->collider.left
							&& actors[i]->collider.top < actors[j]->collider.bottom
							&& actors[i]->collider.bottom > actors[j]->collider.top) {
							//1.2.1) For each of these actors we collide with at "desired new position", add them to our "colliding_actors_this_frame" set...
							actors[i]->actors_collided_this_frame.push_back(actors[j]);
							//1.2.2) ...and add ourselves to the other actor's set.
							actors[j]->actors_collided_this_frame.push_back(actors[i]);
						}
					}
				}


			}
			//1.3) If the colliding_actors_this_frame set has a size larger than 0 (IE, we collided with something)...
			if (actors[i]->actors_collided_this_frame.empty()) {
				//1.3.1) Do not move this frame. (aka do nothing, see else statement in 1.4)
				//1.3.2) Invert our velocity(only meaningful for NPCs).
				actors[i]->velocity *= (-1);
			}
			//1.4) If the colliding_actors_this_frame set has a size of 0, move this actor to "desired new position".
			else {
				updateActorPosition(actors[i], newPos);
				AnimateActor(actors[i], flip);
			}
		}
		//TODO if its player
	}	
}

void Scene::AnimateActor(Actor* actor, bool flip) {
	/*If the actor attempts to move east, the x component should become positive (regardless of its original value).
	If the actor attempts to move west, the x component should become negative (regardless of its original value. */
	if (actor->position.x < getNewPosFromVelocity(actor->position, actor->velocity).x) {
		if (flip) actor->scale.x = std::abs(actor->scale.x);
	}
	else if (actor->position.x > getNewPosFromVelocity(actor->position, actor->velocity).x) {
		if (flip) actor->scale.x = -1 * std::abs(actor->scale.x);
	}

	if (actor->position == getNewPosFromVelocity(actor->position, actor->velocity)) {
		actor->moving = false;
	}
	else actor->moving = true;
	
	// for moving upward / displaying backImage
	if (actor->view_image_back.has_value()) {
		if (actor->position.y < getNewPosFromVelocity(actor->position, actor->velocity).y) {
			actor->currentView = actor->view_image_back.value();
		}
		if (actor->position.y > getNewPosFromVelocity(actor->position, actor->velocity).y) {
			actor->currentView = actor->view_image;
		}
	}
	
}

void Scene::AnimatePlayer(glm::vec2& direction, bool flip) {
	/*If the actor attempts to move east, the x component should become positive (regardless of its original value).
	If the actor attempts to move west, the x component should become negative (regardless of its original value. */
	if (direction.x > 0) {
		if (flip) player->scale.x = std::abs(player->scale.x);
	}
	else if (direction.x < 0) {
		if (flip) player->scale.x = -1 * std::abs(player->scale.x);
	}

	if (direction == glm::vec2{ 0,0 }) player->moving = false;
	else player->moving = true;

	// for moving upward / displaying backImage
	if (player->view_image_back.has_value()) {
		if (direction.y < 0) {
			player->currentView = player->view_image_back.value();
		}
		if (direction.y > 0) {
			player->currentView = player->view_image;
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

	/*//only return the updated position if its not blocked
	if (!CheckBlocking(newPosition)) {
		return newPosition;
		// IF ITS BLOCKED FLIP THE VELOCITY !!!!
	}
	else {
		velocity = -velocity;
	}*/
	
	// return old position if actor wont go anywhere
	return position;
}
