#pragma once
#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::pair <float, float> get_coordinate(std::string now_town);

std::string get_weather(std::pair <float, float> now_coordinate, int count_days);