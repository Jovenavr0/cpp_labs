#pragma once
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void output_to_console(std::string& info_text, json& config, std::string& town_name);