#pragma once
#include <string>


namespace util {
	std::string substrBetween(const std::string& str, const std::string& start, const std::string& end) {
		// Find the starting position of the first substring
		size_t startPos = str.find(start);
		if (startPos == std::string::npos) return "";

		startPos += start.length();

		// Find the position of the second substring after the first substring
		size_t endPos = str.find(end, startPos);
		if (endPos == std::string::npos) return "";

		// Return the substring between the start and end positions
		return str.substr(startPos, endPos - startPos);
	}

}