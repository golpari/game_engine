#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>

#include <filesystem>

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "EngineUtils.h"


class Scene {
public:

	static void GameStart() {

		EngineUtils::CheckPathExists("resources/");
		EngineUtils::CheckPathExists("resources/game.config");

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
};
#endif