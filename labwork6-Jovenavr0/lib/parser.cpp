#include "parser.h"
#include <cstring>

std::vector <Arguments> Parse(int argc, char* argv[]){
	std::vector <Arguments> all_arguments;

	for (int index = 1; index < argc; ++index) {

		if (!strcmp(argv[index], "-c") or !strcmp(argv[index], "--create")) {
			auto* argument = new Arguments("-c");
			all_arguments.push_back(*argument);
		} else if (!strcmp(argv[index], "-f") or !strncmp(argv[index], "--file=", 7)) {
			if (!strcmp(argv[index], "-f")) {
				all_arguments[all_arguments.size() - 1].name_of_archive = argv[++index];
			} else {
				std::string name;
				for (int i = 7; i < strlen(argv[index]); ++i) {
					name.push_back(argv[index][i]);
				}
				all_arguments[all_arguments.size() - 1].name_of_archive = name;
			}
		} else if (!strcmp(argv[index], "-l") or !strcmp(argv[index], "--list")) {
			auto* argument = new Arguments("-l");
			all_arguments.push_back(*argument);
		} else if (!strcmp(argv[index], "-x") or !strcmp(argv[index], "--extract")) {
			auto* argument = new Arguments("-x");
			all_arguments.push_back(*argument);
		} else if (!strcmp(argv[index], "-a") or !strcmp(argv[index], "--append")) {
			auto* argument = new Arguments("-a");
			all_arguments.push_back(*argument);
		} else if (!strcmp(argv[index], "-d") or !strcmp(argv[index], "--delete")) {
			auto* argument = new Arguments("-d");
			all_arguments.push_back(*argument);
		} else if (!strcmp(argv[index], "-A") or !strcmp(argv[index], "--concatenate")) {
			auto* argument = new Arguments("-A");
			argument -> file_name_vector.emplace_back(argv[++index]);
			argument -> file_name_vector.emplace_back(argv[++index]);
			argument -> count_file += 2;
			all_arguments.push_back(*argument);
		} else {
			all_arguments[all_arguments.size() - 1].file_name_vector.emplace_back(argv[index]);
			all_arguments[all_arguments.size() - 1].count_file += 1;
		}

	}

	return all_arguments;
}