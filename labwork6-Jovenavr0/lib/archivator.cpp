#include "archivator.h"
#include "HammingCoding.h"
#include <fstream>

void Actions (const std::vector <Arguments>& all_arguments) {

	int size_of_vector = all_arguments.size();

	for (int index = 0; index < size_of_vector; ++index) {
		if (all_arguments[index].action == "-c") {
			Create(all_arguments[index]);
		} else if (all_arguments[index].action == "-l") {
			List(all_arguments[index]);
		} else if (all_arguments[index].action == "-x") {
			Extract(all_arguments[index]);
		} else if (all_arguments[index].action == "-a") {
			Append(all_arguments[index]);
		} else if (all_arguments[index].action == "-d") {
			Delete(all_arguments[index]);
		} else if (all_arguments[index].action == "-A") {
			Concatenate(all_arguments[index]);
		}
	}

}

void Create (const Arguments& now_argument) {
	InfoHeader info_header;

	info_header.count_of_file = now_argument.file_name_vector.size();
	for (int index = 0; index < info_header.count_of_file; ++index) {
		std::ifstream file( now_argument.file_name_vector[index], std::ios::binary | std::ios::ate);
		info_header.vector_of_size.push_back(file.tellg());
		info_header.vector_of_filename.push_back(now_argument.file_name_vector[index]);
		info_header.vector_of_filename.emplace_back("\\");
		file.close();
	}

	info_header.vector_of_filename.emplace_back("$");

	std::ofstream info_header_file("header.txt", std::ios::binary);
	info_header_file.write((char*)&info_header, sizeof(InfoHeader));
	info_header_file.close();

	Encode_Hamming("header.txt", sizeof(InfoHeader), now_argument.name_of_archive);

	remove("header.txt");

	for (int index = 0; index < info_header.count_of_file; ++index) {
		Encode_Hamming(now_argument.file_name_vector[index], info_header.vector_of_size[index], now_argument.name_of_archive);
	}
}

void Extract (const Arguments& now_argument) {

//	Error_Checker(now_argument.name_of_archive);

	Decode_Info info = Decode_Hamming(now_argument.name_of_archive);
	Arguments argument("-c");
	argument.name_of_archive = now_argument.name_of_archive;
	bool checker = true;
	if (now_argument.count_file) {

		for (uint64_t index = 0; index < info.count_file; ++index) {
			for (uint64_t index_del = 0; index_del < now_argument.count_file; ++index_del) {
				if (info.vector_of_filename[index] == now_argument.file_name_vector[index_del]) {
					checker = false;
				}
			}

			if (checker) {
				argument.file_name_vector.push_back(info.vector_of_filename[index]);
				argument.count_file += 1;
			}

			checker = true;

		}

		Create(argument);

		for (uint64_t index = 0; index < argument.count_file; ++index) {
			remove((char*)&argument.file_name_vector[index]);
		}

	}
}

uint64_t Count (const Arguments& now_argument) {

//	Error_Checker(now_argument.name_of_archive);

	std::ifstream file(now_argument.name_of_archive, std::ios::binary | std::ios::ate);
	uint64_t answer;
	long long index = 1;
	char now_char;

	if (!file) {
		std::cerr << "Error opening archive with name: " << now_argument.name_of_archive;
		exit(EXIT_FAILURE);
	}

	while (index & (index - 1)) {
		file.get(now_char);
		index = (index + 1) % 1024;
	}

	file.get(now_char);
	answer = int(now_char) - 48;

	index = (index + 1) % 1024;

	for (int i = 0; i < 7; ++i) {

		while (index & (index - 1)) {
			file.get(now_char);
			index = (index + 1) % 1024;
		}

		file.get(now_char);
		index = (index + 1) % 1024;

		answer = (answer << 1) + int(now_char) - 48;
	}

	return answer;
}

void List (const Arguments& now_argument) {

//	Error_Checker(now_argument.name_of_archive);

	std::ifstream file(now_argument.name_of_archive, std::ios::binary | std::ios::ate);
	std::string now_file_name;
	uint64_t count_filename = Count(now_argument);
	long long index = 1;
	char now_char;

	if (!file) {
		std::cerr << "Error opening archive with name: " << now_argument.name_of_archive;
		exit(EXIT_FAILURE);
	}

	for (int i = 0 ; i < 16; ++i) {

		while (index & (index - 1)) {
			file.get(now_char);
			index = (index + 1) % 1024;
		}

		file.get(now_char);
		index = (index + 1) % 1024;

	}

	while (count_filename != 0) {
		while (index & (index - 1)) {
			file.get(now_char);
			index = (index + 1) % 1024;
		}

		file.get(now_char);
		index = (index + 1) % 1024;

		if (now_char != '\\') {
			now_file_name.push_back(now_char);
		} else {
			std::cout << now_file_name << '\n';
			--count_filename;
			now_file_name = "";
		}
	}

}

void Append (const Arguments& now_argument) {

//	Error_Checker(now_argument.name_of_archive);

	Decode_Info info = Decode_Hamming(now_argument.name_of_archive);
	Arguments argument("-c");
	argument.name_of_archive = now_argument.name_of_archive;
	argument.file_name_vector = info.vector_of_filename;
	for (int index = 0; index < now_argument.count_file; ++index) {
		argument.file_name_vector.push_back(now_argument.file_name_vector[index]);
	}
	argument.count_file = now_argument.count_file + info.count_file;
	Create(argument);

	for (uint64_t index = 0; index < info.count_file; ++index) {
		remove((char*)&argument.file_name_vector[index]);
	}

}

void Delete (const Arguments& now_argument) {

//	Error_Checker(now_argument.name_of_archive);

	Decode_Info info = Decode_Hamming(now_argument.name_of_archive);
	Arguments argument("-c");
	argument.name_of_archive = now_argument.name_of_archive;
	argument.file_name_vector = info.vector_of_filename;
	argument.count_file = info.count_file;
	for (int index = 0; index < now_argument.count_file; ++index) {
		for (int i = 0; i < info.count_file; ++i) {
			if (info.vector_of_filename[i] == now_argument.file_name_vector[index]) {
				argument.file_name_vector.erase(argument.file_name_vector.begin() + i);
				argument.count_file - 1;
			}
		}
	}
	Create(argument);

	for (uint64_t index = 0; index < argument.count_file; ++index) {
		remove((char*)&argument.file_name_vector[index]);
	}

}

void Concatenate (const Arguments& now_argument) {

//	Error_Checker(now_argument.name_of_archive);

	Decode_Info info_first_archive = Decode_Hamming(now_argument.file_name_vector[0]);
	Decode_Info info_second_archive = Decode_Hamming(now_argument.file_name_vector[1]);
	Arguments argument("-c");
	argument.name_of_archive = now_argument.name_of_archive;
	argument.file_name_vector = info_first_archive.vector_of_filename;
	argument.file_name_vector.insert(argument.file_name_vector.end(), info_second_archive.vector_of_filename.begin(), info_second_archive.vector_of_filename.end());
	argument.count_file = info_first_archive.count_file + info_second_archive.count_file;
	Create(argument);

	for (uint64_t index = 0; index < argument.count_file; ++index) {
		remove((char*)&argument.file_name_vector[index]);
	}

}