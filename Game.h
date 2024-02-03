#ifndef GAME_H
#define GAME_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "EngineUtils.h"
#include "rapidjson/document.h"
#include "Scene.h"


class Game {	

public:
	std::vector<Scene> scenes;

	Scene* currentScene;

	rapidjson::Document out_gameConfig;

	std::stringstream ss;
	bool firstRun = true;
	int health = 3;
	int score = 0;

	void GameStart();

	std::string GameEnd(bool good);

	static uint64_t GetCameraResolution();

	void LoadInitialScene(rapidjson::Document& out_gameConfig);

	std::string CheckDialogue(std::string& dialogue, bool& scoredUpped);

	std::string PrintDialogue(Scene& scene);

	void RunScene(Scene& scene, std::string& input);
};
#endif