#ifndef ACTORTEMPLATE_H
#define ACTORTEMPLATE_H

#include <string>
#include <optional>

#include "glm/glm.hpp"

class ActorTemplate
{
public:
	std::string name = "";
	float pos_x = 0;
	float pos_y = 0;
	float vel_y = 0;
	float vel_x = 0;
	char view = '?';
	std::string nearby_dialogue = "";
	std::string contact_dialogue = "";
	bool scoredUpped = false;

	std::string view_image = "";
	double scaleX = 1.0;// TODO: use std::abs() when importing new scales
	double scaleY = 1.0;
	// TODO: negative scale shld cause actor to 'flip' -- use SDL_RendererFlip
	double rotation = 0.0;
	double pivot_offsetX = 0.0; // ACTUAL DEFAUlt iS 0.5*w and 0.5*h !!! TODO: process differently
	double pivot_offsetY = 0.0; //TODO STD OPTOINAL
	std::optional<double> render_order;
	std::string view_image_back;
	bool movementBounce = false;

	ActorTemplate(std::string actor_name_in, float pos_x_in, float pos_y_in, float vel_x_in, float vel_y_in,
		std::string nearby_dialogue_in, std::string contact_dialogue_in,
		std::string view_image_in, double scaleX_in, double scaleY_in, double rotation_in, 
		double pivot_offsetX_in, double pivot_offsetY_in, std::optional<double> render_order_in, std::string view_image_back_in, bool movementBounce_in)
		: name(std::move(actor_name_in)), // Use std::move for strings
		pos_x(pos_x_in), 
		pos_y(pos_y_in),
		vel_x(vel_x_in),
		vel_y(vel_y_in),
		/*view(view_in),*/
		nearby_dialogue(std::move(nearby_dialogue_in)), // Use std::move for strings
		contact_dialogue(std::move(contact_dialogue_in)), // Use std::move for strings
		scoredUpped(false), // Directly initialize to false

		// new parameters for hw5
		view_image(view_image_in),
		scaleX(scaleX_in),
		scaleY(scaleY_in),
		rotation(rotation_in),
		pivot_offsetX(pivot_offsetX_in),
		pivot_offsetY(pivot_offsetY_in),
		render_order(render_order_in),
		view_image_back(view_image_back_in),
		movementBounce(movementBounce_in)
	{}


	ActorTemplate() {}
};
#endif