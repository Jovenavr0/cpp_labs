#pragma once
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

json parse_config();