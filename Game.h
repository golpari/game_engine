#ifndef GAME_H
#define GAME_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "ActorTemplate.h"
#include "EngineUtils.h"
#include "rapidjson/document.h"
#include "Scene.h"
#include "Renderer.h"
#include "SDL_ttf.h"
#include "AudioHelper.h"

class Scene;
class Renderer;

class Game {	

public:
	std::vector<Scene> scenes;
	rapidjson::Document out_gameConfig;

	std::vector<std::string> introImages;
	std::vector<std::string> introTexts;
	TTF_Font* font;

	Scene* currentScene = nullptr;

	std::stringstream ss;
	bool firstRun = true;
	int health = 3;
	int score = 0;
	bool loadNew = false;
	std::string nextScene = "";

	Game() { currentScene = nullptr; }

	void GameStart();

	void ProcessIntro();

	void PlayGameplayAudio();

	std::string GameEnd(bool good);

	void LoadInitialScene();

	void LoadScene(std::string sceneName);

	std::string CheckDialogue(std::string& dialogue, bool& scoredUpped);

	std::string PrintDialogue();

	void RunScene();

private:
	bool introAudioPlaying = false;

	void Deallocate();

	void RunIntro(int& index, Renderer& renderer, bool& playAudio);

	std::string HudSetup();

	void RenderAll(Renderer& renderer);
};
#endif