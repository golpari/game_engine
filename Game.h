#ifndef GAME_H
#define GAME_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "EngineUtils.h"
#include "rapidjson/document.h"
#include "Scene.h"
#include "ActorTemplate.h"

class Scene;

class Game {	

public:
	std::vector<Scene> scenes;

	Scene* currentScene;
	
	rapidjson::Document out_gameConfig;

	std::stringstream ss;
	bool firstRun = true;
	int health = 3;
	int score = 0;
	bool loadNew = false;
	std::string nextScene = "";

	Game() {} 

	void GameStart();

	std::string GameEnd(bool good);

	static uint64_t GetCameraResolution();

	void LoadInitialScene(rapidjson::Document& out_gameConfig);

	void LoadScene(std::string sceneName);

	std::string CheckDialogue(std::string& dialogue, bool& scoredUpped);

	std::string PrintDialogue();

	void RunScene();

private:
	void Deallocate();
};
#endif