#include "Game.h"

void Game::GameStart() {

	EngineUtils::CheckPathExists("resources/", true);
	EngineUtils::CheckPathExists("resources/game.config", true);

	EngineUtils::ReadJsonFile("resources/game.config", out_gameConfig);

	this->LoadInitialScene(out_gameConfig);

	if (out_gameConfig.HasMember("game_start_message")) {
		std::cout << out_gameConfig["game_start_message"].GetString() << "\n";
	}

}

std::string Game::GameEnd(bool good) {
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
	int x_res = 13;
	int y_res = 9;

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

	//check for initial scene
	if (out_gameConfig.HasMember("initial_scene")) {
		std::string temp = (out_gameConfig["initial_scene"].GetString());
		if (!temp.empty()) {

			std::string sceneName = out_gameConfig["initial_scene"].GetString();
			LoadScene(sceneName);
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

void Game::LoadScene(std::string sceneName)
{
	rapidjson::Document out_sceneInitial;
	std::filesystem::path scenePath = "resources/scenes/" + sceneName + ".scene";
	if (EngineUtils::CheckPathExists(scenePath, false)) {
		EngineUtils::ReadJsonFile("resources/scenes/" + sceneName + ".scene", out_sceneInitial);

		if (currentScene != nullptr) {
			// deallocate mem for all the actors in the old scene
			currentScene->actors.clear();
			currentScene->player = nullptr;
			currentScene->actors_map.clear();
			
			// deallocate mem of the old scene
			delete currentScene;
		}
		
		currentScene = new Scene(); // store scene on the heap
		currentScene->ProcessActors(out_sceneInitial);
	}

	else {
		std::cout << "error: scene " << sceneName << " is missing";
		exit(0);
	}
}

std::string Game::CheckDialogue(std::string& dialogue, bool& scoredUpped) {
	if (dialogue.find("proceed to") != std::string::npos) {
		loadNew = true;
		nextScene = EngineUtils::obtain_word_after_phrase(dialogue, "proceed to ");
	}
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

std::string Game::PrintDialogue() {
	std::vector<Dialogue> dialogues;

	//for nearby dialogue
	int x, y = 0;
	std::string endgameString = "";

	// Split the player's position into x and y coordinates
	EngineUtils::split(currentScene->player->position, x, y);

	// Offsets for the surrounding 8 spaces
	const std::vector<glm::ivec2> offsets = {
		{ -1, -1 }, { 0, -1 }, { 1, -1 },
		{ -1,  0 },            { 1,  0 },
		{ -1,  1 }, { 0,  1 }, { 1,  1 }
	};

	Dialogue temp;

	// if there is anyone nearby, the player will also be added to the nearby dialogue list
	bool playerDialogued = false;

	// calculate every option for surrounding positions
	for (const auto& offset : offsets) {
		// Calculate the position for each surrounding space
		int checkX = x + offset.x;
		int checkY = y + offset.y;

		// Combine the coordinates back into a uint64_t position
		uint64_t adjacent = EngineUtils::combine(checkX, checkY);

		// check actors at adjacent position
		auto actorsIt = currentScene->actors_map.find(adjacent);
		if (actorsIt != currentScene->actors_map.end()) {
			// found actors for this position, add them to the printable list
			for (std::shared_ptr<Actor> actor : actorsIt->second) {
				//check nearby dialogue
				if (actor->actor_name != "player" && adjacent == actor->position && actor->nearby_dialogue != "" && actor->nearby_dialogue != " ") {
					temp.dialogueID = actor->actorID;
					temp.text = actor->nearby_dialogue;
					dialogues.push_back(temp);
					endgameString = CheckDialogue(actor->nearby_dialogue, actor->scoredUpped);
				}
			}
			/*if (!playerDialogued) {
				actorsWithNearbyDialogue.push_back(scene.player);
				playerDialogued = true;
			}*/
		}
	}

	// for contact dialogue
	auto actorsIt = currentScene->actors_map.find(currentScene->player->position);
	if (actorsIt != currentScene->actors_map.end()) {
		// found actors for this position, add them to the printable list
		for (std::shared_ptr<Actor> actor : actorsIt->second) {
			// check nearby dialogue
			if (actor->actor_name != "player" && currentScene->player->position == actor->position && actor->contact_dialogue != "" && actor->contact_dialogue != " ") {
				temp.dialogueID = actor->actorID;
				temp.text = actor->contact_dialogue;
				dialogues.push_back(temp);
				endgameString = CheckDialogue(actor->contact_dialogue, actor->scoredUpped);
			}
		}
	}
	
	// sort the dialogues to be printed by actorID (which is the same as dialogueID)
	std::sort(dialogues.begin(), dialogues.end(), DialogueComparator());

	//print the dialogues in order by actorID
	for (Dialogue& dialogue : dialogues) {
		ss << dialogue.text << "\n";
	}

	return endgameString;
}

void Game::RunScene(Scene& scene)
{
	std::string input;
	do {
		//update player position based on the movement
		if (!firstRun) scene.MoveActors();
		firstRun = false;

		scene.MovePlayer(input);

		currentScene->RenderScene();

		// check + print nearby & contact dialogue & eng game if needed
		std::string endgame = PrintDialogue();

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

		if (loadNew) {
			loadNew = false;
			std::cout << ss.str();
			ss.clear();
			ss.str("");
			LoadScene(nextScene);
			currentScene->RenderScene();

			//prompt user
			ss << "Please make a decision..." << '\n';

			//explain user options
			ss << "Your options are \"n\", \"e\", \"s\", \"w\", \"quit\"" << '\n';

			std::cout << ss.str();
			ss.clear();
			ss.str("");
		}
		else {
			//prompt user
			ss << "Please make a decision..." << '\n';

			//explain user options
			ss << "Your options are \"n\", \"e\", \"s\", \"w\", \"quit\"" << '\n';

			std::cout << ss.str();
			ss.clear();
			ss.str("");
		}
	} while (std::cin >> input && input != "quit");
}
