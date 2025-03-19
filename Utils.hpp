#pragma once

#include <string>
#include <algorithm>

namespace Utils {
	std::string ToLower(std::string Str) {
		std::transform(Str.begin(), Str.end(), Str.begin(), ::tolower);
		return Str;
	}
}