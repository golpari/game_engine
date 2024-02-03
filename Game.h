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
	std::vector<Scene*> scenes;

	static void GameStart() {

		EngineUtils::CheckPathExists("resources/", true);
		EngineUtils::CheckPathExists("resources/game.config", true);

		rapidjson::Document out_gameConfig;
		EngineUtils::ReadJsonFile("resources/game.config", out_gameConfig);
		if (out_gameConfig.HasMember("game_start_message")) {
			std::cout << out_gameConfig["game_start_message"].GetString() << "\n";
		}

	}

	static std::string GameEnd(bool good) {
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

	static uint64_t GetCameraResolution() {
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

	Scene LoadInitialScene() {
		scenes.reserve(10); // come back to this later
		rapidjson::Document out_sceneInitial;
		rapidjson::Document out_gameConfig;
		EngineUtils::ReadJsonFile("resources/game.config", out_gameConfig);

		//check for initial scene
		if (out_gameConfig.HasMember("initial_scene")) {
			if (out_gameConfig["initial_scene"].GetString() != "") {

				std::string sceneName = out_gameConfig["initial_scene"].GetString();
				std::filesystem::path scenePath = "resources/scenes/" + sceneName + ".scene";
				if (EngineUtils::CheckPathExists(scenePath, false)) {
					EngineUtils::ReadJsonFile("resources/scenes/" + sceneName + ".scene", out_sceneInitial);

					//take this out of the if later?

					Scene initialScene;
					initialScene.ProcessActors(out_sceneInitial);
					scenes.push_back(&initialScene);
					return initialScene;
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

	 void RunScene(Scene& scene) {

	 }
};
#endif