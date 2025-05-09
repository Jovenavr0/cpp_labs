#include "HammingCoding.h"
#include <cmath>
#include <fstream>
#include <cstdio>

void Encode_Hamming (std::string file_name, uint64_t count_byte_in_file, std::string name_of_archive) {
	std::vector <uint8_t> for_haf;
	std::vector <uint8_t> data_mas;
	int inspect_mas[11] = {0};
	int counter_data;
	int counter_inspect;
	uint8_t  last_bit;
	uint8_t value;
	int count_sign;
	uint64_t count_block = (count_byte_in_file - 1) / 126 + 1;
	uint8_t now_char;

	std::ofstream haf_file(name_of_archive, std::ios::binary | std::ios::app);

	std::ifstream now_file(file_name, std::ios_base::binary);
	if (!now_file) {
		std::cerr << "Error opening file with name: " << file_name;
		exit(EXIT_FAILURE);
	}

	for (uint64_t number = 0; number < count_block; ++number) {

		counter_data = 0;

		if (number != (count_block - 1)) {

			for (int i = 0; i < 126; ++i) {
				now_char = now_file.get();
				value = int(now_char);
				for (int j = 0; j < 8; j++) {
					data_mas.push_back((value >> (j * 1)) % 2);
				}
			}

			for (uint64_t index = 1; index < 1019; ++index) {
				if (index & (index - 1)) {
					for (int i = 0; i < 10; i++) {
						if (index >> (i * 1) >= 1) {
							if ((index >> (i * 1)) % 2 == 1) {
								inspect_mas[i] ^= data_mas[counter_data];
								++counter_data;
							}
						} else {
							break;
						}
					}
				}
			}

			counter_data = 0;
			counter_inspect = 0;
			last_bit = 0;

			for (uint64_t index = 1; index < 1019; ++index) {
				if (index & (index - 1)) {
					for_haf.push_back(data_mas[counter_data]);
					last_bit ^= data_mas[counter_data];
					++counter_data;
				} else {
					for_haf.push_back(inspect_mas[counter_inspect]);
					last_bit ^= inspect_mas[counter_inspect];
					++counter_inspect;
				}
			}

			for_haf.push_back(last_bit);

			haf_file.write((char*)&for_haf, 1019 * sizeof(uint8_t));

			for_haf.clear();
			data_mas.clear();
			for (int i = 0; i < 11; ++i) {
				inspect_mas[i] = 0;
			}

		} else {


			while (!now_file.eof()) {
				now_char = now_file.get();
				value = int(now_char);
				if (!now_file.eof()) {
					for (int j = 0; j < 8; j++) {
						data_mas.push_back((value >> (j * 1)) % 2);
					}
				}

			}

			count_sign = data_mas.size();
			counter_data = 0;

			for (uint64_t index = 1; index <= count_sign; ++index) {
				if (index & (index - 1)) {
					for (int i = 0; i < 10; i++) {
						if (index >> (i * 1) >= 1) {
							if ((index >> (i * 1)) % 2 == 1) {
								inspect_mas[i] ^= data_mas[counter_data];
							}
						} else {
							break;
						}
					}
					++counter_data;
				} else {
					++count_sign;
				}
			}

			counter_data = 0;
			counter_inspect = 0;
			last_bit = 0;

			for (uint64_t index = 1; index <= count_sign; ++index) {
				if (index & (index - 1)) {
					for_haf.push_back(data_mas[counter_data]);
					last_bit ^= data_mas[counter_data];
					++counter_data;
				} else {
					for_haf.push_back(inspect_mas[counter_inspect]);
					last_bit ^= inspect_mas[counter_inspect];
					++counter_inspect;
				}
			}

			for_haf.push_back(last_bit);
			++count_sign;

			for (int i = 0; i < for_haf.size(); ++i) {
				haf_file << for_haf[i];
			}

			for_haf.clear();
			data_mas.clear();
			for (int i = 0; i < 11; ++i) {
				inspect_mas[i] = 0;
			}

		}

	}

	now_file.close();
	haf_file.close();

}

Decode_Info Decode_Hamming (std::string file_name) {

	std::vector <uint64_t> vector_of_size;
	std::vector <std::string> vector_of_filename;
	std::string now_file_name;
	uint64_t now_size;
	uint64_t count_file = 0;
	uint64_t count_filename;
	long long index = 1;
	uint8_t now_char;
	int now_byte;
	int now_value;
	uint8_t value;
	int rang;
	int count_rang;
	int size_rang;

	std::ifstream file(file_name, std::ios_base::binary | std::ios::in);

	if (!file) {
		std::cerr << "Error opening archive with name: " << file_name;
		exit(EXIT_FAILURE);
	}

	count_rang = 0;

	for (int i = 0; i < 8; i++) {

		rang = 0;
		now_value = 0;

		for (int j = 0; j < 8; ++j) {

			while (!(index & (index - 1))) {
				now_char = file.get();
				index = (index + 1) % 1019;
			}

			now_char = file.get();
			value = int(now_char);
			now_value += (value * pow(2, rang));
			++rang;
			index = (index + 1) % 1019;

		}

		count_file += (now_value * int(pow(2, count_rang)));
	}

	count_filename = count_file;

	for (uint64_t number_file = 0; number_file < count_file; ++number_file) {

		now_size = 0;

		for (int i = 0; i < 8; i++) {

			rang = 0;

			for (int j = 0; j < 8; ++j) {
				while (!(index & (index - 1))) {
					now_char = file.get();
					index = (index + 1) % 1019;
				}

				now_char = file.get();
				value = int(now_char);
				now_value += (value * pow(2, rang));
				++rang;
				index = (index + 1) % 1019;

			}

			now_size += (now_value * int(pow(2, i)));
		}

		vector_of_size.push_back(now_size);
	}

	while (count_filename != 0) {

		now_byte = 0;
		rang = 0;

		for (int j = 0; j < 8; ++j) {

			while (!(index & (index - 1))) {
				now_char = file.get();
				index = (index + 1) % 1019;
			}

			now_char = file.get();
			value = int(now_char);
			now_byte += (value * pow(2, rang));
			++rang;
			index = (index + 1) % 1019;

		}

		if (now_byte != 92) {
			now_file_name.push_back(char(now_byte));
		} else {
			vector_of_filename.push_back(now_file_name);
			--count_filename;
			now_file_name = "";
		}
	}

	for (int j = 0; j < 8; ++j) {

		while (!(index & (index - 1))) {
			now_char = file.get();
			index = (index + 1) % 1019;
		}

		now_char = file.get();
		index = (index + 1) % 1019;

	}

	index = 1;

	for (uint64_t file_number = 0; file_number < count_file; ++file_number) {

		std::ofstream now_file(vector_of_filename[file_number], std::ios::binary| std::ios::app);

		now_size = vector_of_size[file_number];
		while (now_size) {

			now_byte = 0;
			rang = 0;

			for (int j = 0; j < 8; ++j) {

				while (!(index & (index - 1))) {
					now_char = file.get();
					index = (index + 1) % 1019;
				}

				now_char = file.get();
				value = int(now_char);
				now_byte += (value * pow(2, rang));
				++rang;
				index = (index + 1) % 1019;

			}

			now_file << char(now_byte);

			--now_size;
		}

		now_file.close();

	}

	Decode_Info info;
	info.vector_of_filename = vector_of_filename;
	info.count_file = count_file;
	info.vector_of_size = vector_of_size;

	return info;
}

void Error_Checker (std::string name_of_archive)  {
	std::ifstream file(name_of_archive, std::ios::binary);
	char now_bit;
	int rang;
	int now_byte;
	int value;
	uint64_t error_index = 0;
	int inspect_mas[11] = {0};
	int block_bit[1019];
	uint64_t index = 1;
	uint64_t now_position = 1;

	while (file.get(now_bit)) {

		now_byte = int(now_bit) - 48;
		block_bit[index - 1] = int(now_bit) - 48;
		inspect_mas[10] ^= int(now_bit) - 48;
		rang = 1;

		for (int j = 0; j < 7; ++j) {

			while (!(index & (index - 1))) {
				file.get(now_bit);
				block_bit[index - 1] = int(now_bit) - 48;
				index = (index + 1) % 1019;
			}

			file.get(now_bit);
			value = int(now_bit) - 48;
			for (int i = 0; i < 11; i++) {
				if (index >> (i * 1) >= 1) {
					if ((index >> (i * 1)) % 2 == 1) {
						inspect_mas[i] ^= value;
					}
				} else {
					break;
				}
			}
			block_bit[index - 1] = value;
			inspect_mas[10] ^= value;
			now_byte += int(value * pow(2, rang));
			++rang;
			index = (index + 1) % 1019;

		}

		++now_position;

		if (index == 0) {
			++index;
			file.get(now_bit);
			block_bit[1018] = int(now_bit) - 48;

			for (int i = 0; i < 10; ++i) {
				inspect_mas[10] ^= inspect_mas[i];
				if (inspect_mas[i] != block_bit[int(pow(2, i)) - 1]) {
					error_index += int(pow(2, i));
				}
			}

			if (error_index != 0) {
				if (block_bit[1018] == inspect_mas[10]) {
					std::cerr << "Critical damage, it is impossible to restore the archive: " << name_of_archive;
					exit(EXIT_FAILURE);
				} else {
					Change_Broken_Bit(name_of_archive, now_position - 1019 + error_index - 1,
									  block_bit[error_index - 1]);
					std::cerr << "An error was found and successfully fixed" << '\n';
				}
			}


			for (int i = 0; i < 11; ++i) {
				inspect_mas[i] = 0;
			}
		}

		if (char(now_byte) == '$') {
			index = 1;
			++now_position;
			continue;
		}

		while (index & (index - 1)) {
			file.get(now_bit);
			block_bit[index - 1] = int(now_bit) - 48;
			index = (index + 1) % 1019;
		}

	}

}

void Change_Broken_Bit (std::string name_of_archive, uint64_t cursor_position, int32_t value) {
	std::string new_name = name_of_archive;
	char now_char;
	uint64_t now_position = 0;
	new_name += "B";
	std::rename((char*)&name_of_archive, (char*)&new_name);
	std::fstream fix_file(name_of_archive, std::ios::binary);
	std::ofstream new_file(new_name, std::ios::binary);

	while (fix_file.get(now_char)) {
		new_file << now_char;
	}

	fix_file.close();
	new_file.close();

	std::ofstream file(name_of_archive, std::ios::binary);
	std::ifstream broken_file(new_name, std::ios::binary);

	while (broken_file.get(now_char)) {
		now_position += 1;
		if (now_position == cursor_position) {
			value ^= 1;
			file << value;
		} else {
			file << now_char;
		}
	}

	broken_file.close();
	file.close();

	std::remove((char*)&new_name);

}