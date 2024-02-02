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

	//from chat
	// Function to combine two uint32_t values into a single uint64_t
	static uint64_t combine(uint32_t& x, uint32_t& y) {
		// Shift the first uint32_t 32 bits to the left and combine with the second
		return (static_cast<uint64_t>(x) << 32) | y;
	}

	//from chat
	// Function to split a uint64_t back into two uint32_t values
	static void split(const uint64_t combined, uint32_t& x, uint32_t& y) {
		// Right shift to get the high 32 bits
		x = combined >> 32;
		// Bitwise AND with 0xFFFFFFFF to get the low 32 bits
		y = combined & 0xFFFFFFFF;
	}
};
#endif