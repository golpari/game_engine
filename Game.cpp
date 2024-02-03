#include "Game.h"

void Game::GameStart() {

	EngineUtils::CheckPathExists("resources/", true);
	EngineUtils::CheckPathExists("resources/game.config", true);

	rapidjson::Document out_gameConfig;
	EngineUtils::ReadJsonFile("resources/game.config", out_gameConfig);

	this->LoadInitialScene(out_gameConfig);

	if (out_gameConfig.HasMember("game_start_message")) {
		std::cout << out_gameConfig["game_start_message"].GetString() << "\n";
	}

}

std::string Game::GameEnd(bool good) {
	// how to resetup this to not have the parsing happen twice? compared to gameStart
	rapidjson::Document out_gameConfig;
	EngineUtils::ReadJsonFile("resources/game.config", out_gameConfig);
	if (good)
	{
		if (out_gameConfig.HasMember("game_over_good_message")) {
			return out_gameConfig["game_over_good_message"].GetString();
		}
		else {
			exit(0);
		}
	}
	else {
		if (out_gameConfig.HasMember("game_over_bad_message")) {
			return out_gameConfig["game_over_bad_message"].GetString();
		}
		else {
			exit(0);
		}
	}
}

uint64_t Game::GetCameraResolution() {
	//default values
	uint32_t x_res = 13;
	uint32_t y_res = 9;

	if (EngineUtils::CheckPathExists("resources/rendering.config", false)) {
		rapidjson::Document out_renderingConfig;
		EngineUtils::ReadJsonFile("resources/rendering.config", out_renderingConfig);
		if (out_renderingConfig.HasMember("x_resolution")) {
			x_res = out_renderingConfig["x_resolution"].GetUint();
		}
		if (out_renderingConfig.HasMember("y_resolution")) {
			y_res = out_renderingConfig["y_resolution"].GetUint();
		}
	}

	return EngineUtils::combine(x_res, y_res);
}

void Game::LoadInitialScene(rapidjson::Document& out_gameConfig) {
	rapidjson::Document out_sceneInitial;
	EngineUtils::ReadJsonFile("resources/game.config", out_gameConfig);

	//check for initial scene
	if (out_gameConfig.HasMember("initial_scene")) {
		std::string temp = (out_gameConfig["initial_scene"].GetString());
		if (!temp.empty()) {

			std::string sceneName = out_gameConfig["initial_scene"].GetString();
			std::filesystem::path scenePath = "resources/scenes/" + sceneName + ".scene";
			if (EngineUtils::CheckPathExists(scenePath, false)) {
				EngineUtils::ReadJsonFile("resources/scenes/" + sceneName + ".scene", out_sceneInitial);

				//take this out of the if later?

				Scene initialScene;
				scenes.push_back(initialScene);
				scenes.back().ProcessActors(out_sceneInitial);
				currentScene = &scenes.back();
			}

			else {
				std::cout << "error: scene " << out_gameConfig["initial_scene"].GetString() << " is missing";
				exit(0);
			}
		}
		else {
			std::cout << "error: initial_scene unspecified";
			exit(0);
		}
	}
	else {
		std::cout << "error: initial_scene unspecified";
		exit(0);
	}
}

std::string Game::CheckDialogue(std::string& dialogue, bool& scoredUpped) {
	if (dialogue.find("game over") != std::string::npos)
		return Game::GameEnd(false);
	if (dialogue.find("health down") != std::string::npos)
		health--;
	if (dialogue.find("score up") != std::string::npos && !scoredUpped) {
		score++;
		scoredUpped = true;
	}
	if (dialogue.find("you win") != std::string::npos)
		return Game::GameEnd(true);
	if (health <= 0)
		return Game::GameEnd(false);
	return "";
}

std::string Game::PrintDialogue(Scene& scene) {
	//access surrounding 8 slots
	const int diffX[8] = { -1, -1, -1, 0, 1, 1, 1, 0 };
	const int diffY[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };

	int adjacentX;
	int adjacentY;
	std::string endgameString;
	//loop through possible actors
	for (Actor actor : scene.actors) {
		//print contact dialogue if relevant
		if (scene.player->position == actor.position && actor.contact_dialogue != "") {
			std::cout << actor.contact_dialogue << '\n';
			endgameString = CheckDialogue(actor.contact_dialogue, actor.scoredUpped);
		}

		// loop through adjacent actors
		for (int i = 0; i < 8; i++) {
			glm::ivec2 adjacent{ scene.player->position.x + diffX[i], scene.player->position.y + diffY[i] };

			//print nearby dialogue if relevant
			if (adjacent == actor.position && actor.nearby_dialogue != "") {
				std::cout << actor.nearby_dialogue << '\n';
				endgameString = CheckDialogue(actor.nearby_dialogue, actor.scoredUpped);
			}
		}
	}
	return endgameString;
}

void Game::RunScene(Scene& scene, std::string input)
{
	//update player position based on the movement
	if (!firstRun) scene.MoveActors();
	firstRun = false;

	scene.MovePlayer(input);

	currentScene->RenderScene();

	// check + print nearby & contact dialogue & eng game if needed
	std::string endgame = PrintDialogue(*currentScene);

	// print player health and score
	ss << "health : " << health << ", "
		<< "score : " << score << '\n';

	//if relevant, print relevant game end string  and return
	if (endgame != "") {
		ss << endgame;
		std::cout << ss.str();
		ss.clear();
		ss.str("");
		exit(0);
	}

	//prompt user
	ss << "Please make a decision..." << '\n';

	//explain user options
	ss << "Your options are \"n\", \"e\", \"s\", \"w\", \"quit\"" << '\n';

	std::cout << ss.str();
	ss.clear();
	ss.str("");
}
