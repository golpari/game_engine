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
};
#endif