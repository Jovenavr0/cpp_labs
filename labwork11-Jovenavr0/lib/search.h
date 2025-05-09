#pragma once
#include <vector>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "revindex.h"

void Search(int argc, char** argv, const names& naming);

int64_t Search_world(const std::string& word, const std::string& filename, uint64_t TID_count);

void Score_world(const std::string& word, const names& naming, std::unordered_map<uint64_t, double>& all_files);