#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <optional>
#include <vector>

#include "glm/glm.hpp"

extern int guuid;

struct BoxCollider {
	float top = 0;
	float bottom = 0;
	float left = 0;
	float right = 0;
};

class Actor
{
public:
	// TODO: comment this out
	std::string actor_name;
	int actorID;
	glm::vec2 position;
	glm::vec2 velocity;
	//char view;
	BoxCollider collider;
	std::string nearby_dialogue;
	std::string contact_dialogue;
	bool scoredUpped;

	std::string view_image = "";
	glm::vec2 scale = { 1.0, 1.0 };
	double rotation = 0.0;
	std::optional<double> pivot_offsetX;
	std::optional<double> pivot_offsetY;

	std::optional<double> render_order;
	std::optional<std::string> view_image_back;
	std::string currentView = ""; //this is to track which view is being displayed at any given time 
	//-- actor pivot always rendered wrt view_image but thats not always the one displayed
	bool movement_bounce_enabled = false;
	bool moving = false;

	std::vector<Actor*> actors_collided_this_frame;

	Actor(std::string actor_name_in, /*char view_in, */glm::vec2 position_in, glm::vec2 initial_velocity_in,
		BoxCollider collider_in, std::string nearby_dialogue_in, std::string contact_dialogue_in,
		std::string view_image_in, glm::vec2 scale_in, double rotation_in, std::optional<double> pivot_offsetX_in,
		std::optional<double> pivot_offsetY_in, std::optional<double> render_order_in, std::optional<std::string> view_image_back_in, bool movement_bounce_in)
		: actor_name(std::move(actor_name_in)), // Use std::move for strings
		actorID(guuid++), // Directly initialize to 0
		position(std::move(position_in)), // Use std::move if glm::vec2 has a move constructor; otherwise, it's fine as is
		velocity(std::move(initial_velocity_in)), // Same as position
		/*view(view_in),*/
		collider(collider_in),
		nearby_dialogue(std::move(nearby_dialogue_in)), // Use std::move for strings
		contact_dialogue(std::move(contact_dialogue_in)), // Use std::move for strings
		scoredUpped(false), // Directly initialize to false

		// new parameters for hw5
		view_image(view_image_in),
		scale(scale_in),
		rotation(rotation_in),
		pivot_offsetX(pivot_offsetX_in),
		pivot_offsetY(pivot_offsetY_in),
		render_order(render_order_in),
		view_image_back(view_image_back_in),
		currentView(view_image_in),
		movement_bounce_enabled(movement_bounce_in)
	{}
};
#endif