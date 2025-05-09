#include "parser.h"
#include <cstring>
#include <fstream>
#include <string>
#include <sstream>

int KCountBitByte = 8;

Options ParseArguments(int argc, char** argv) {
    Options element;

    for (int i = 1; i < argc; ++i) {

        if (!strcmp(argv[i], "-i")) {
			if (++i < argc) {
				element.input_directory = argv[i];
				continue;
			} else {
				std::cerr << "Please enter correct input directory";
				exit(EXIT_FAILURE);
			}
        }

        else if (!strncmp("--input=", argv[i], 8)) {

            int len_argument = strlen(argv[i]) - 8;
            char* input_directory = new char[len_argument + 1];
            input_directory[len_argument] = '\0';

            for (int j = 0; j < len_argument; j++) {
                input_directory[j] = argv[i][8 + j];
            }
            element.input_directory = input_directory;
            continue;
        }

        else if (!strcmp(argv[i], "-o")) {
			if (++i < argc) {
				element.output_directory = argv[i];
				continue;
			} else {
				std::cerr << "Please enter correct output directory";
				exit(EXIT_FAILURE);
			}
        }

        else if (!strncmp("--output=", argv[i], 9)) {

            int len_argument = strlen(argv[i]) - 9;
            char* output_directory = new char[len_argument + 1];
            output_directory[len_argument] = '\0';

            for (int j = 0; j < len_argument; j++) {
                output_directory[j] = argv[i][9 + j];
            }

            element.output_directory = output_directory;

            continue;
        }

        else if (!strcmp(argv[i], "-m")) {
            try {
                element.max_iter = std::stoll(argv[++i]);
            }
            catch (std::logic_error) {
                std::cerr << "Please enter correct maximum counter iteration\n";
                exit(EXIT_FAILURE);
            }
            continue;
        }

        else if (!strncmp("--max-iter=", argv[i], 11)) {

            int len_argument = strlen(argv[i]) - 11;
            char* max_iter = new char[len_argument];

            for (int j = 0; j < len_argument; j++) {
                max_iter[j] = argv[i][11 + j];
            }
            element.max_iter = std::stoll(max_iter);
			delete[] max_iter;
            continue;
        }

        else if (!strcmp(argv[i], "-f")) {
            try {
                element.frequency = std::stoll(argv[++i]);
            }
            catch (std::logic_error) {
                std::cerr << "Please enter correct frequency\n";
                exit(EXIT_FAILURE);
            }
            continue;
        }

        else if (!strncmp("--freq=", argv[i], 7)) {

            int len_argument = strlen(argv[i]) - 7;
            char* frequency = new char[len_argument];

            for (int j = 0; j < len_argument; j++) {
                frequency[j] = argv[i][7 + j];
            }
            element.frequency = std::stoll(frequency);
			delete[] frequency;
            continue;
        }

    }

    return element;
}

ParseSize GetSizes(const char* filename) {

    ParseSize size;
    std::ifstream file(filename);
	int16_t coordinate_x;
	int16_t coordinate_y;
	uint64_t count_sand;

    if (!file.is_open()) {
		std::cerr << "Please enter correct filename";
		exit(EXIT_FAILURE);
	}

	while (file >> coordinate_x >> coordinate_y >> count_sand){
		if (coordinate_x > size.max_x) {
			size.max_x = coordinate_x;
		}
		if (size.min_x > coordinate_x) {
			size.min_x = coordinate_x;
		}
		if (coordinate_y > size.max_y) {
			size.max_y = coordinate_y;
		}
		if (size.min_y > coordinate_y) {
			size.min_y = coordinate_y;
		}
	}

    return size;
}

MainPointField FillField(Options element, MainPointField field) {
	std::ifstream file(element.input_directory);
	std::string now_str;
	int16_t coordinate_x;
	int16_t coordinate_y;
	uint64_t count_sand;

	field.mas = new uint64_t*[field.y];

	for (int i = 0; i < field.y; i++) {
		field.mas[i] = new uint64_t[field.x];
		memset(field.mas[i], 0, field.x * KCountBitByte);
	}

	if (!file.is_open()) {
		std::cerr << "Please enter correct filename";
		exit(EXIT_FAILURE);
	}

	while (file >> coordinate_x >> coordinate_y >> count_sand){
		field.mas[coordinate_y - field.start_y][coordinate_x - field.start_x] = count_sand;
	}

	field.output_directory = element.output_directory;
	field.frequency = element.frequency;
	field.max_iter = element.max_iter;

	 if (!field.max_iter) {
		field.max_iter = std::numeric_limits<uint64_t>::max();
	}

	return field;
}

std::ostream& operator<<(std::ostream& stream, const MainPointField& field) {
	for (int i = 0; i < field.y; i++) {
		for (int j = 0; j < field.x; j++) {
			stream << field.mas[i][j] << ' ';
		}
		stream << '\n';
	}
	stream << '\n';

	return stream;
}
