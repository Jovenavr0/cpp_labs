#pragma once
#include <set>
#include <cmath>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <unordered_map>

struct names{
	std::string index;
	std::string terms;
	std::string files;
	uint64_t count;
	uint64_t avgdl;
	uint64_t count_TID;
};

names MakeRevIndex(const std::string& dir_name);

void ConcatenateBlocks(uint64_t count_block, const std::string& name);