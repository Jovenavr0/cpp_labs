#include "revindex.h"
#include "console.h"
#include "search.h"
#include <iostream>

void RunIndex(char** argv) {
	std::cout << "Enter the FULL PATH to the root folder of the search area and your request:\n";
	std::cout << "Please wait until the necessary files are prepared for the search.\n";

	std::string dir_name(argv[1]);
	names naming = MakeRevIndex(dir_name);

	std::ofstream file_naming("NAMING", std::ios::binary);
	for (int i = 0; i < 8; ++i) {
		file_naming.put(static_cast<char>(naming.count % 256));
		naming.count = (naming.count >> 8);
	}

	for (int i = 0; i < 8; ++i) {
		file_naming.put(static_cast<char>(naming.avgdl % 256));
		naming.avgdl = (naming.avgdl >> 8);
	}

	for (int i = 0; i < 8; ++i) {
		file_naming.put(static_cast<char>(naming.count_TID % 256));
		naming.count_TID = (naming.count_TID >> 8);
	}
	file_naming.close();
}

void RunSearch(int argc, char** argv) {
	std::cout << "Enter your request:\n";

	char block;
	uint64_t count = 0;
	uint64_t avgdl = 0;
	uint64_t count_TID = 0;

	std::ifstream file_naming("NAMING", std::ios::binary);
	for (int i = 0; i < 8; ++i) {
		file_naming.get(block);
		count += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, i));
	}
	for (int i = 0; i < 8; ++i) {
		file_naming.get(block);
		avgdl += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, i));
	}
	for (int i = 0; i < 8; ++i) {
		file_naming.get(block);
		count_TID += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, i));
	}
	file_naming.close();

	names naming{"INDEXMAP", "TERMS", "DID", count, avgdl, count_TID};

	Search(argc, argv, naming);

}