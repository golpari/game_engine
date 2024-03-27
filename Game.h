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
#include "Input.h"

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

	Game() { currentScene = nullptr; }

	void GameStart();

	void ProcessIntro();

	void PlayGameplayAudio();

	std::string GameEnd(bool good);

	void LoadInitialScene();

	void LoadScene(std::string sceneName);

	void RunScene();

private:

	void Deallocate();

	void RunIntro(int& index, Renderer& renderer, bool& playAudio);

	std::string HudSetup();

	void RenderAll(Renderer& renderer);

	bool StartFrame(int& index, Renderer& renderer, bool playScene);
};
#endif
