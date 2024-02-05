#ifndef ENGINEUTILS_H
#define ENGINEUTILS_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "Actor.h"
#include "ActorTemplate.h"

extern std::unordered_map<std::string, ActorTemplate*> templates;

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

	static bool CheckPathExists(std::filesystem::path path, const bool errExit) {
		// Check if directory or file is missing
		if (!std::filesystem::exists(path)) {
			if (errExit) {
				std::cout << "error: " << path.string() << " missing";
				exit(0);
			}
			return false;
		}
		return true;
	}

	// can change this to not always load templates and to only load them when they are requested
	// check if template was already processed, if no, check path exists and process it. 
	static void ProcessTemplate(std::string templateName) {
		//if the folder exists, then load the template
		if (EngineUtils::CheckPathExists("resources/actor_templates/" + templateName + ".template", false)) {
			//parse the template into readable json
			rapidjson::Document out_template;
			EngineUtils::ReadJsonFile("resources/actor_templates/" + templateName + ".template", out_template);

			// process the template

			std::string name = "";
			int x = 0;
			int y = 0;
			int vel_x = 0;
			int vel_y = 0;
			char view = '?';
			bool blocking = false;
			std::string nearby_dialogue = "";
			std::string contact_dialogue = "";

			if (out_template.HasMember("name")) { name = out_template["name"].GetString(); }
			if (out_template.HasMember("x")) { x = out_template["x"].GetInt(); }
			if (out_template.HasMember("y")) { y = out_template["y"].GetInt(); }
			if (out_template.HasMember("vel_x")) { vel_x = out_template["vel_x"].GetInt(); }
			if (out_template.HasMember("vel_y")) { vel_y = out_template["vel_y"].GetInt(); }
			if (out_template.HasMember("view")) { view = *out_template["view"].GetString(); }
			if (out_template.HasMember("blocking")) { blocking = out_template["blocking"].GetBool(); }
			if (out_template.HasMember("nearby_dialogue")) { nearby_dialogue = out_template["nearby_dialogue"].GetString(); }
			if (out_template.HasMember("contact_dialogue")) { contact_dialogue = out_template["contact_dialogue"].GetString(); }

			// create template variable
			ActorTemplate* new_template = new ActorTemplate(name, view, x, y, vel_x, vel_y, blocking, nearby_dialogue, contact_dialogue);
			// store template in map of templates
			templates[templateName] = new_template;
		}
		else {
			std::cout << "error: template " << templateName << " is missing";
			exit(0);
		}
	}

	//from chat
	// Function to combine two uint32_t values into a single uint64_t
	static uint64_t combine(int& x, int& y) {
		// cast to ensure the ints become exactly 32 bits in size.
		uint32_t ux = static_cast<uint32_t>(x);
		uint32_t uy = static_cast<uint32_t>(y);

		// place x into right 32 bits.
		uint64_t result = static_cast<uint64_t>(ux);

		// move x to left 32 bits.
		result = result << 32;

		// place y into right 32 bits.
		result = result | static_cast<uint64_t>(uy);

		return result;
	}

	//from chat
	// Function to split a uint64_t back into two uint32_t values
	static void split(const uint64_t combined, int& x, int& y) {
		// Extract the upper 32 bits for x
		x = static_cast<int>((combined >> 32) & 0xFFFFFFFF);
		// Extract the lower 32 bits for y
		y = static_cast<int>(combined & 0xFFFFFFFF);
	}

	static std::string obtain_word_after_phrase(const std::string& input, const std::string& phrase) {
		// Find the position of the phrase in the string
		size_t pos = input.find(phrase);

		//If phrase is not found, return an empty string
		if (pos == std::string::npos) return "";

		//Find the starting position of the next word (skip spaces after the phrase)
		pos += phrase.length();
		while (pos < input.size() && std::isspace(input[pos])) {
			++pos;
		}

		//If we're at the end of the string, return an empty string
		if (pos == input.size())  return "";

		// Find the end position of the word (until a space or the end of the string)
		size_t endPos = pos;
		while (endPos < input.size() && !std::isspace(input[endPos])) {
			++endPos;
		}

		// extract and return the word
		return input.substr(pos, endPos - pos);
	}
};

struct Dialogue {
	std::string text = "";
	int dialogueID = 0;
};

struct ActorComparator {
	bool operator()(const std::shared_ptr<Actor> a, const std::shared_ptr<Actor> b) const {
		return a->actorID < b->actorID;
	}
};

struct DialogueComparator {
	bool operator()(const Dialogue a, const Dialogue b) const {
		return a.dialogueID < b.dialogueID;
	}
};
#endif