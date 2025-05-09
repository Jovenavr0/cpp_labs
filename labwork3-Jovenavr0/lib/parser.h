#pragma once
#include <cinttypes>
#include <iostream>
#include <limits>

struct Options {
    char* input_directory;
    char* output_directory;
    int64_t frequency = 0;
    uint64_t max_iter = std::numeric_limits<uint64_t>::max();
//	~Options(){
//		delete input_directory;
//		delete output_directory;
//	}
};

struct ParseSize {
    int16_t max_x = 0;
    int16_t max_y = 0;
	int16_t min_x = std::numeric_limits<int16_t>::max();
	int16_t min_y = std::numeric_limits<int16_t>::max();
};

struct MainPointField {
	MainPointField(ParseSize size): x(size.max_x - size.min_x + 1), y(size.max_y - size.min_y + 1), start_x(size.min_x), start_y(size.min_y)
	{}
	int16_t x;
	int16_t start_x;
	int16_t y;
	int16_t start_y;
	uint64_t** mas = nullptr;
	char* output_directory = nullptr;
	int64_t frequency;
	uint64_t max_iter;
//	~MainPointField(){
//		delete output_directory;
//		for(int i  = 0; i < y; i++){
//			delete[] mas[i];
//		}
//		delete mas;
//	}
};

Options ParseArguments(int argc, char** argv);

ParseSize GetSizes(const char* filename);

MainPointField FillField(Options, MainPointField);

std::ostream& operator<<(std::ostream& stream, const MainPointField&);
