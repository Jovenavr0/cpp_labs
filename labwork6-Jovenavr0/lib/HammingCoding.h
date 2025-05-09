#pragma once

#include <iostream>
#include <cstdint>
#include <vector>

#pragma pack(1)
struct Decode_Info{
	uint64_t count_file;
	std::vector <std::string> vector_of_filename;
	std::vector <uint64_t> vector_of_size;
};
#pragma pack()

void Encode_Hamming (std::string file_name, uint64_t count_byte_in_file, std::string name_of_archive);

Decode_Info Decode_Hamming (std::string file_name);

void Error_Checker (std::string name_of_archive);

void Change_Broken_Bit (std::string name_of_archive, uint64_t cursor_position, int32_t value);