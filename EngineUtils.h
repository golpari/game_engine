#ifndef ENGINEUTILS_H
#define ENGINEUTILS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

#include <filesystem>

class EngineUtils {
public:

	static void ReadJsonFile(const std::string& path, rapidjson::Document& out_document)
	{
		FILE* file_pointer = nullptr;
#ifdef _WIN32
		fopen_s(&file_pointer, path.c_str(), "rb");
#else
		file_pointer = fopen(path.c_str(), "rb");
#endif
		char buffer[65536];
		rapidjson::FileReadStream stream(file_pointer, buffer, sizeof(buffer));
		out_document.ParseStream(stream);
		std::fclose(file_pointer);

		if (out_document.HasParseError()) {
			rapidjson::ParseErrorCode errorCode = out_document.GetParseError();
			std::cout << "error parsing json at [" << path << "]" << std::endl;
			exit(0);
		}
	}

	static void CheckResourcesAndConfig() {
		// Check if resources directory missing
		if (!std::filesystem::exists("resources/")) {
			std::cout << "error: resources/ missing";
			exit(0);
		}

		// Check if gameConfig missing
		if (!std::filesystem::exists("resources/game.config")) {
			std::cout << "error: resources/game.config missing";
			exit(0);
		}
	}
	static void GameStart() {

		CheckResourcesAndConfig();
		
		rapidjson::Document out_gameConfig;
		ReadJsonFile("resources/game.config", out_gameConfig);
		if (out_gameConfig.HasMember("game_start_message")) {
			if (out_gameConfig["game_start_message"] != "") {
				std::cout << out_gameConfig["game_start_message"].GetString() << "\n";
			}
		}
		
	}
};
#endif