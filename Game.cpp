#include "Game.h"

bool StartFrame(std::vector<std::string>& introImages, int& index, Renderer& renderer);


void Game::GameStart() {

	EngineUtils::CheckPathExists("resources/", true);
	EngineUtils::CheckPathExists("resources/game.config", true);

	EngineUtils::ReadJsonFile("resources/game.config", out_gameConfig);

	/*this->LoadInitialScene(out_gameConfig);

	if (out_gameConfig.HasMember("game_start_message")) {
		std::cout << out_gameConfig["game_start_message"].GetString() << "\n";
	}*/

}

void Game::ProcessIntro() {
	//process intro images
	if (out_gameConfig.HasMember("intro_image")
		&& out_gameConfig["intro_image"].IsArray()
		&& !out_gameConfig["intro_image"].GetArray().Empty()) {
		introImages.reserve(out_gameConfig["intro_image"].GetArray().Size());
		for (const auto& img : out_gameConfig["intro_image"].GetArray()) {
			std::string imageName = img.GetString();
			introImages.push_back(imageName);
			if (!EngineUtils::CheckPathExists("resources/images/" + imageName + ".png", false)) {
				std::cout << "error: missing image " << imageName;
				exit(0);
			}
		}

		// only process intro text if intro image is defined
		if (out_gameConfig.HasMember("intro_text") && out_gameConfig["intro_text"].IsArray()) {
			introTexts.reserve(out_gameConfig["intro_text"].GetArray().Size());
			for (const auto& txt : out_gameConfig["intro_text"].GetArray()) {
				std::string textName = txt.GetString();
				introTexts.push_back(textName);
			}

			// only error font spec if intro text specified but font not
			if (!out_gameConfig.HasMember("font")) {
				std::cout << "error: text render failed. No font configured";
				exit(0);
			}
		}
	}

	// check font existence if it is specified
	if (out_gameConfig.HasMember("font")) {
		std::string fontName = out_gameConfig["font"].GetString();
		if (!EngineUtils::CheckPathExists("resources/fonts/" + fontName + ".ttf", false)) {
			std::cout << "error: font " << fontName << " missing";
			exit(0);
		}
		else {
			TTF_Init();
			font = TTF_OpenFont(("resources/fonts/" + fontName + ".ttf").c_str(), 16);
		}
	}

	// check / process audio intro
	if (out_gameConfig.HasMember("intro_bgm")) {
		std::string audioName = out_gameConfig["intro_bgm"].GetString();
		// if it doesnt exist as a wav file
		if (!EngineUtils::CheckPathExists("resources/audio/" + audioName + ".wav", false)) {
			// AND it doesnt exist as a ogg file then error
			if (!EngineUtils::CheckPathExists("resources/audio/" + audioName + ".ogg", false)) {
				std::cout << "error: failed to play audio clip " << audioName;
				exit(0);
			}
			// if it DOES exist as an ogg, then play it as an ogg
			else {
				//AudioHelper::Mix_LoadWAV498(("resources/audio/" + audioName + ".ogg").c_str());
				//AudioHelper::Mix_AllocateChannels498(0);
				AudioHelper::Mix_PlayChannel498(0,
					AudioHelper::Mix_LoadWAV498(("resources/audio/" + audioName + ".ogg").c_str()),
					-1);
				introAudioPlaying = true;
			}
		}
		// if it DOES exist as a wav, then play it as a wav. We assume that no 2 sameName ogg and wav files exist
		else {
			//AudioHelper::Mix_LoadWAV498(("resources/audio/" + audioName + ".wav").c_str());
			//AudioHelper::Mix_AllocateChannels498(0);
			AudioHelper::Mix_PlayChannel498(0,
				AudioHelper::Mix_LoadWAV498(("resources/audio/" + audioName + ".wav").c_str()),
				-1);
			introAudioPlaying = true;
		}

	}


}

void Game::PlayGameplayAudio() {
	// check for and process the audio
	if (out_gameConfig.HasMember("gameplay_audio")) {
		std::string audioName = out_gameConfig["gameplay_audio"].GetString();
		// if it doesnt exist as a wav file
		if (!EngineUtils::CheckPathExists("resources/audio/" + audioName + ".wav", false)) {
			// AND it doesnt exist as a ogg file then error
			if (!EngineUtils::CheckPathExists("resources/audio/" + audioName + ".ogg", false)) {
				std::cout << "error: failed to play audio clip " << audioName;
				exit(0);
			}
			// if it DOES exist as an ogg, then play it as an ogg
			else {
				//AudioHelper::Mix_LoadWAV498(("resources/audio/" + audioName + ".ogg").c_str());
				//AudioHelper::Mix_AllocateChannels498(0);
				AudioHelper::Mix_PlayChannel498(0,
					AudioHelper::Mix_LoadWAV498(("resources/audio/" + audioName + ".ogg").c_str()),
					-1);
			}
		}
		// if it DOES exist as a wav, then play it as a wav. We assume that no 2 sameName ogg and wav files exist
		else {
			//AudioHelper::Mix_LoadWAV498(("resources/audio/" + audioName + ".wav").c_str());
			//AudioHelper::Mix_AllocateChannels498(0);
			AudioHelper::Mix_PlayChannel498(0,
				AudioHelper::Mix_LoadWAV498(("resources/audio/" + audioName + ".wav").c_str()),
				-1);
		}
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

void Game::LoadInitialScene() {

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
			Deallocate();
		}

		Scene initialScene;
		scenes.push_back(initialScene);
		scenes.back().ProcessActors(out_sceneInitial);
		currentScene = &scenes.back();

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
	x = currentScene->player->position.x;
	y = currentScene->player->position.y;

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
		glm::vec2 adjacent{ checkX, checkY };

		// check actors at adjacent position
		auto actorsIt = currentScene->actors_map.find(adjacent);
		if (actorsIt != currentScene->actors_map.end()) {
			// found actors for this position, add them to the printable list
			for (Actor* actor : actorsIt->second) {
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
		for (Actor* actor : actorsIt->second) {
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

void Game::RunScene()
{
	Renderer renderer;

	//get title, otherwise default to ""
	std::string title = "";
	if (out_gameConfig.HasMember("game_title")) {
		title = out_gameConfig["game_title"].GetString();
	}

	ProcessIntro();
	LoadInitialScene();
	int index = 0;
	bool playAudio = false;
	bool playScene = false;

	renderer.Initialize(title);
	while (true) {

		// start frame and process events
		if (!StartFrame(introImages, index, renderer)) {
			// in case of exit window event being triggered
			RenderAll(renderer);
			renderer.EndFrame();
			exit(0);
		}

		// DO STUFF!!! 
		// 
		// show intro image as directed
		if (!playScene)
			RunIntro(index, renderer, playAudio);

		if (playAudio) {
			PlayGameplayAudio();
			playAudio = false;
			playScene = true;
		}

		if (playScene) {
			RenderAll(renderer);
		}

		renderer.EndFrame();
	}
	/*std::string input;
	do {


		//update player position based on the movement
		if (!firstRun) currentScene->MoveActors();
		firstRun = false;

		currentScene->MovePlayer(input);

		//currentScene->RenderScene();

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

			// check + print nearby & contact dialogue & eng game if needed
			std::string endgame = PrintDialogue();

			// print player health and score
			ss << "health : " << health << ", "
				<< "score : " << score << '\n';

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
	} while (std::cin >> input && input != "quit");*/
}

//PRIVATE HELPER
void Game::Deallocate() {
	// Deallocate memory pointed to by the pointers in the unordered_map
	for (auto& map_entry : currentScene->actors_map) {
		for (Actor* actor : map_entry.second) {
			delete actor; // Deallocate memory for each Actor in the vector
		}
		map_entry.second.clear(); // Clear the vector (not strictly necessary)
	}
	currentScene->actors_map.clear(); // Clear the map (not strictly necessary)

	//// Deallocate memory pointed to by the pointers in the vector
	//for (Actor* actor : currentScene->actors) {
	//	delete actor; // Deallocate memory for each Actor in the vector
	//}
	currentScene->actors.clear(); // Clear the vector (not strictly necessary)

	// Deallocate memory pointed to by the 'player' pointer
	//delete currentScene->player; // Deallocate memory for the player
	currentScene->player = nullptr;
}

// returns filename of the hp image, if no hp img defined, then exits with error
std::string Game::HudSetup()
{
	if (!out_gameConfig.HasMember("hp_image")) {
		std::cout << "error: player actor requires an hp_image be defined";
		exit(0);
	}
	return out_gameConfig["hp_image"].GetString();
}

void Game::RenderAll(Renderer& renderer)
{
	// render all actors
	for (Actor* actor : currentScene->actors) {
		renderer.RenderActor(*actor, { 0, 0 });
	}
	// render HUD icons and text
	if (currentScene->player != nullptr) {
		renderer.RenderHUD(HudSetup(), font, health, score);
	}
}

void Game::RunIntro(int& index, Renderer& renderer, bool& playAudio) {
	if (introImages.empty()) {
		playAudio = true;
		return;
	}
	int w = 0;
	int h = 0;
	if (introImages.size() > index && introTexts.size() > index) {
		renderer.RenderImage(introImages[index]);
		TTF_SizeText(font, introTexts[index].c_str(), &w, &h);
		renderer.RenderText(font, introTexts[index], 16, SDL_Color{ 255, 255, 255, 255 }, w, h);
	}
	else if (introTexts.size() > index && !introImages.empty()) {
		renderer.RenderImage(introImages[introImages.size() - 1]);
		TTF_SizeText(font, introTexts[index].c_str(), &w, &h);
		renderer.RenderText(font, introTexts[index], 16, SDL_Color{ 255, 255, 255, 255 }, w, h);
	}
	else if (introImages.size() > index && !introTexts.empty()) {
		renderer.RenderImage(introImages[index]);
		TTF_SizeText(font, introTexts[introTexts.size() - 1].c_str(), &w, &h);
		renderer.RenderText(font, introTexts[introTexts.size() - 1], 16, SDL_Color{ 255, 255, 255, 255 }, w, h);
	}
	else if (introImages.size() > index && introTexts.empty()) {
		renderer.RenderImage(introImages[index]);
	}
	// if done with intro, show plain screen and end intro audio
	else
	{
		SDL_RenderClear(renderer.renderer);
		if (introAudioPlaying) {
			AudioHelper::Mix_HaltChannel498(0);
			introAudioPlaying = false;
		}
		playAudio = true;
	}
}

bool StartFrame(std::vector<std::string>& introImages, int& index, Renderer& renderer)
{
	// Check Events
	SDL_Event nextEvent;
	while (Helper::SDL_PollEvent498(&nextEvent)) {
		if (nextEvent.type == SDL_QUIT) {
			SDL_SetRenderDrawColor(renderer.renderer, renderer.r, renderer.g, renderer.b, 255);
			if (introImages.empty())
				SDL_RenderClear(renderer.renderer);
			// render all actors
			//for (Actor* actor : actors) {
			//    RenderActor(*actor, { 0, 0 });
			//}
			//EndFrame();
			//exit(0);
			return false;
		}

		// Mouse event: SDL_MOUSEBUTTONDOWN is for mouse button press
		else if (nextEvent.type == SDL_MOUSEBUTTONDOWN && nextEvent.button.button == SDL_BUTTON_LEFT) {
			// Handle left mouse click
			index++;
		}

		// Keyboard event: SDL_KEYDOWN is for key press
		else if (nextEvent.type == SDL_KEYDOWN) {
			// Check for spacebar or enter key using scancode
			if (nextEvent.key.keysym.scancode == SDL_SCANCODE_SPACE || nextEvent.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				// Handle spacebar or enter key press
				index++;
			}
			// do move player stuff TODO
		}
	}

	SDL_SetRenderDrawColor(renderer.renderer, renderer.r, renderer.g, renderer.b, 255);
	SDL_RenderClear(renderer.renderer);
	return true;
}