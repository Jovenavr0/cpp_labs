#pragma once

#include <string>
#include <vector>
#include <cstdint>

#pragma pack(1)
struct Arguments{
	std::string name_of_archive;
	std::string action;
	uint64_t count_file = 0;
	std::vector <std::string> file_name_vector;

	Arguments(std::string now_action): action(now_action)
	{}

};
#pragma pack()

std::vector <Arguments> Parse(int argc, char* argv[]);