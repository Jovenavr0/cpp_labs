#pragma once
#include "parser.h"
#include <iostream>
#include <vector>
#include <cstdint>

#pragma pack(1)
struct InfoHeader {
	uint64_t count_of_file;
	std::vector <uint64_t> vector_of_size;
	std::vector <std::string> vector_of_filename;
};
#pragma pack()

void Actions (const std::vector <Arguments>& all_arguments);

void Create (const Arguments& now_argument);
void Extract (const Arguments& now_argument);
uint64_t Count (const Arguments& now_argument);
void List (const Arguments& now_argument);
void Append (const Arguments& now_argument);
void Delete (const Arguments& now_argument);
void Concatenate (const Arguments& now_argument);

