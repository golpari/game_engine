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
#include "AudioHelper.h"
#include "EngineUtils.h"
#include "rapidjson/document.h"
#include "Renderer.h"
#include "Scene.h"

#if __APPLE__
    #include "SDL2_ttf/SDL_ttf.h"
#else
    #include "SDL_ttf.h"
#endif

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
	int cooldownPoint = -180;
	bool firstRun = true;
	int health = 3;
	int score = 0;
	bool loadNew = false;
	std::string nextScene = "";
	bool win = false;
	bool lose = false;

	std::string badImage = "";
	std::string goodImage = "";
	std::string badAudio = "";
	std::string goodAudio = "";

	Game() { currentScene = nullptr; }

	void GameStart();

	void ProcessIntro();

	void PlayGameplayAudio();

	std::string GameEnd(bool good);

	void LoadInitialScene();

	void LoadScene(std::string sceneName);

	int CheckDialogue(std::string& dialogue, bool& scoredUpped);

	int PrintDialogue(Renderer& renderer);

	void RunScene();

private:
	bool introAudioPlaying = false;

	void Deallocate();

	void RunIntro(int& index, Renderer& renderer, bool& playAudio);

	std::string HudSetup();

	void RenderAll(Renderer& renderer);
};
#endif
