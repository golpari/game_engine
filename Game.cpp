#include "Game.h"

const int COOLDOWN = 180;
const int PLAY = 0;
const int LOSE = 1;
const int WIN = 2;

void Game::GameStart() {

	ComponentManager::Initialize();

	EngineUtils::CheckPathExists("resources/", true);
	EngineUtils::CheckPathExists("resources/game.config", true);
	EngineUtils::CheckPathExists("resources/component_types/", false);

	EngineUtils::ReadJsonFile("resources/game.config", out_gameConfig);
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

void Game::RunScene()
{
	Input::Init(); //setup the keycodes

	Renderer renderer; // want a diff renderer for every scene

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

	//if no intro, then jump right to playScene
	if (introImages.empty()) {
		playScene = true;
		playAudio = true;
	}

	renderer.Initialize(title);
	//every frame starts here
	while (true) {
		// loop through components and run their OnStart the frame after they've been loaded, then clear whatever needs to get run
		for (const auto& entry : currentScene->onStarts) {
			(*entry)["OnStart"](*entry);
		}
		currentScene->onStarts.clear();

		if (!StartFrame(index, renderer, playScene)) {
			// in case of exit window event being triggered
			SDL_SetRenderDrawColor(renderer.renderer, renderer.r, renderer.g, renderer.b, 255);	

			Helper::SDL_RenderPresent498(renderer.renderer);//renderer.EndFrame();
			exit(0);
		}

		if (!win && !lose) {
			// start frame and process events

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

			if (loadNew && !win && !lose) {
				loadNew = false;
				SDL_RenderClear(renderer.renderer);
				LoadScene(nextScene);
				RenderAll(renderer);
			}
		}

		if (win) {
			SDL_RenderClear(renderer.renderer);
		}
		else if (lose) {
			SDL_RenderClear(renderer.renderer);
		}

		Helper::SDL_RenderPresent498(renderer.renderer);//renderer.EndFrame();
	}
}

//private helpers
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
	/*** render all actors in their render order ***/

	renderer.RenderActors(currentScene->actors);
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
		renderer.RenderText(font, introTexts[index], 16, SDL_Color{ 255, 255, 255, 255 }, 1, 0);
	}
	else if (introTexts.size() > index && !introImages.empty()) {
		renderer.RenderImage(introImages[introImages.size() - 1]);
		TTF_SizeText(font, introTexts[index].c_str(), &w, &h);
		renderer.RenderText(font, introTexts[index], 16, SDL_Color{ 255, 255, 255, 255 }, 1, 0);
	}
	else if (introImages.size() > index && !introTexts.empty()) {
		renderer.RenderImage(introImages[index]);
		TTF_SizeText(font, introTexts[introTexts.size() - 1].c_str(), &w, &h);
		renderer.RenderText(font, introTexts[introTexts.size() - 1], 16, SDL_Color{ 255, 255, 255, 255 }, 1, 0);
	}
	else if (introImages.size() > index && introTexts.empty()) {
		renderer.RenderImage(introImages[index]);
	}
	// if done with intro, show plain screen and end intro audio
	else
	{
		SDL_RenderClear(renderer.renderer);
		playAudio = true;
	}
}

bool Game::StartFrame(int& index, Renderer& renderer, bool playScene)
{
	// Check Events
	SDL_Event nextEvent;
	glm::vec2 playerDir;
	Input::Update();
	while (Helper::SDL_PollEvent498(&nextEvent)) {

		// process each event to update the keys
		Input::ProcessEvent(nextEvent);

		if (nextEvent.type == SDL_QUIT) {
			
			//std::cout << Helper::GetFrameNumber();
			return false; // so that it is known to dump and end the frame for closeout event
		}

		// Mouse event: SDL_MOUSEBUTTONDOWN is for mouse button press
		else if (nextEvent.type == SDL_MOUSEBUTTONDOWN && nextEvent.button.button == SDL_BUTTON_LEFT) {
			// Handle left mouse click
			index++;
		}
	}

	Input::Update();

	// check keycodes
	{
		// Check for spacebar or enter key using scancode
		if (Input::GetKeyDown(SDL_SCANCODE_SPACE) || Input::GetKeyDown(SDL_SCANCODE_RETURN)) {
			// Handle spacebar or enter key press
			index++;
		}
		// do move player stuff
		playerDir = glm::vec2(0, 0);
		{
			if (Input::GetKey(SDL_SCANCODE_LEFT) || Input::GetKey(SDL_SCANCODE_A)) {
				playerDir += glm::vec2(-1, 0);
			}
			if (Input::GetKey(SDL_SCANCODE_RIGHT) || Input::GetKey(SDL_SCANCODE_D)) {
				playerDir += glm::vec2(1, 0);
			}
			if (Input::GetKey(SDL_SCANCODE_UP) || Input::GetKey(SDL_SCANCODE_W)) {
				playerDir += glm::vec2(0, -1);
			}
			if (Input::GetKey(SDL_SCANCODE_DOWN) || Input::GetKey(SDL_SCANCODE_S)) {
				playerDir += glm::vec2(0, 1);
			}
		}

	}

	SDL_SetRenderDrawColor(renderer.renderer, renderer.r, renderer.g, renderer.b, 255);
	SDL_RenderClear(renderer.renderer);
	return true;
}