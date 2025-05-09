#include "revindex.h"

uint64_t UPPER_BOUND = (2 << 20);

names MakeRevIndex(const std::string& dir_name) {

	if (!std::filesystem::exists(dir_name)) {
		std::cerr << "Enter the correct path to perform the search";
		exit(EXIT_FAILURE);
	}

	std::unordered_map<uint64_t, std::unordered_map<uint64_t, std::vector<uint64_t>>> information;
	std::unordered_map<std::string, uint64_t> terms;
	uint64_t current_DID = 0;
	uint64_t count_block = 0;
	uint64_t current_term;
	uint64_t count_words = 0;
	uint64_t current_count;
	uint64_t numb_terms = 0;
	std::string first_name("DID");
	std::string second_name("TERMS");
	std::string third_name("INDEXMAP");
	std::ofstream DID_file(first_name, std::ios::binary);
	uint64_t len_dir = dir_name.size();
	using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

	for (const auto& dir_entry : recursive_directory_iterator(dir_name)) {

		if (std::filesystem::is_directory(dir_entry)) {
			continue;
		}

		if (sizeof(information) >= UPPER_BOUND) {

			++count_block;
			std::ofstream INDEXMAP_file(third_name + std::to_string(count_block), std::ios::binary);

			for (const auto& el: information) {

				uint64_t TID = el.first;
				for (int i = 0; i < 8; ++i) {
					INDEXMAP_file.put(static_cast<char>(TID % 256));
					TID = (TID >> 8);
				}

				uint64_t term_all_size = 0;
				for (const auto& file: el.second) {
					term_all_size += (2 + file.second.size());
				}
				term_all_size *= 8;
				for (int i = 0; i < 8; ++i) {
					INDEXMAP_file.put(static_cast<char>(term_all_size % 256));
					term_all_size = (term_all_size >> 8);
				}

				for (const auto& file: el.second) {
					uint64_t DID = file.first;
					for (int i = 0; i < 8; ++i) {
						INDEXMAP_file.put(static_cast<char>(DID % 256));
						DID = (DID >> 8);
					}

					uint64_t file_all_size  = file.second.size() * 8;
					for (int i = 0; i < 8; ++i) {
						INDEXMAP_file.put(static_cast<char>(file_all_size % 256));
						file_all_size = (file_all_size >> 8);
					}

					for (const auto& data: file.second) {
						uint64_t str = data;
						for (int i = 0; i < 8; ++i) {
							INDEXMAP_file.put(static_cast<char>(str % 256));
							str = (str >> 8);
						}
					}

				}

			}

			information.clear();
			INDEXMAP_file.close();

		}

		current_count = 0;
		std::string current_name(dir_entry.path().string());
		std::ifstream current_file(current_name, std::ios::binary);
		uint64_t current_size;
		uint64_t number_of_string = 1;
		char symbol;
		std::string word("");
		++current_DID;
		current_name = current_name.substr(len_dir, current_name.size());
		current_size = current_name.size();

		uint64_t DID = current_DID;
		for (int i = 0; i < 8; ++i) {
			DID_file.put(static_cast<char>(DID % 256));
			DID = (DID >> 8);
		}

		uint64_t SIZE = current_size;
		for (int i = 0; i < 8; ++i) {
			DID_file.put(static_cast<char>(SIZE % 256));
			SIZE = (SIZE >> 8);
		}

		for (auto el: current_name) {
			DID_file.put(el);
		}

		if (!current_file.is_open()) {
			std::cerr << "File with name " << current_name << "can't be opened.\n";
			std::cerr << "Fix it and repeat the request.";
			exit(EXIT_FAILURE);
		}

		while (current_file.get(symbol)) {

			if (symbol == '\n') {
				++number_of_string;
			} else if (!isalnum(symbol) && symbol != '_') {

				if (word.empty()) {
					continue;
				}
				if (!terms.contains(word)) {
					++numb_terms;
					terms[word] = numb_terms;
				}
				current_term = terms[word];
				information[current_term][current_DID].push_back(number_of_string);
				++count_words;
				++current_count;
				word.clear();

			} else {
				word += symbol;
			}

		}

		if (!word.empty()) {
			if (!terms.contains(word)) {
				++numb_terms;
				terms[word] = numb_terms;
			}
			current_term = terms[word];
			information[current_term][current_DID].push_back(number_of_string);
			++count_words;
			++current_count;
		}

		for (int i = 0; i < 8; ++i) {
			DID_file.put(static_cast<char>(current_count % 256));
			current_count = (current_count >> 8);
		}

	}

	if (!information.empty()) {

		++count_block;
		std::ofstream INDEXMAP_file(third_name + std::to_string(count_block), std::ios::binary);

		for (const auto& el: information) {

			uint64_t TID = el.first;
			for (int i = 0; i < 8; ++i) {
				INDEXMAP_file.put(static_cast<char>(TID % 256));
				TID = (TID >> 8);
			}

			uint64_t term_all_size = 0;
			for (const auto& file: el.second) {
				term_all_size += (2 + file.second.size());
			}
			term_all_size *= 8;
			for (int i = 0; i < 8; ++i) {
				INDEXMAP_file.put(static_cast<char>(term_all_size % 256));
				term_all_size = (term_all_size >> 8);
			}

			for (const auto& file: el.second) {
				uint64_t DID = file.first;
				for (int i = 0; i < 8; ++i) {
					INDEXMAP_file.put(static_cast<char>(DID % 256));
					DID = (DID >> 8);
				}

				uint64_t file_all_size  = file.second.size() * 8;
				for (int i = 0; i < 8; ++i) {
					INDEXMAP_file.put(static_cast<char>(file_all_size % 256));
					file_all_size = (file_all_size >> 8);
				}

				for (const auto& data: file.second) {
					uint64_t str = data;
					for (int i = 0; i < 8; ++i) {
						INDEXMAP_file.put(static_cast<char>(str % 256));
						str = (str >> 8);
					}

				}

			}

		}

		information.clear();
		INDEXMAP_file.close();
	}

	ConcatenateBlocks(count_block, third_name);

	std::ofstream TERMS_file(second_name, std::ios::binary);

	std::unordered_map<uint64_t, uint64_t> pointer_map;
	std::ifstream IndexMap_file(third_name, std::ios::binary);
	uint64_t pointer = 0;
	uint64_t term_id;
	char block;
	uint64_t term_len;

	while (IndexMap_file.get(block)) {

		term_id = static_cast<uint8_t>(block);
		for (int i = 1; i < 8; ++i) {
			IndexMap_file.get(block);
			term_id += (static_cast<uint8_t>(block) * pow(256, i));
		}

		pointer_map[term_id] = pointer;

		term_len = 0;
		for (int i = 0; i < 8; ++i) {
			IndexMap_file.get(block);
			term_len += (static_cast<uint8_t>(block) * pow(256, i));
		}
		IndexMap_file.seekg(term_len, std::ios_base::cur);
		pointer += 16;
		pointer += term_len;

	}

	IndexMap_file.close();

	for (const auto& term: terms) {

		uint64_t term_size = term.first.size();
		for (int i = 0; i < 8; ++i) {
			TERMS_file.put(static_cast<char>(term_size % 256));
			term_size = (term_size >> 8);
		}

		for (auto el: term.first) {
			TERMS_file.put(el);
		}

		pointer = pointer_map[term.second];

		for (int i = 0; i < 8; ++i) {
			TERMS_file.put(static_cast<char>(pointer % 256));
			pointer = (pointer >> 8);
		}

	}

	TERMS_file.close();

	return names{third_name, second_name, first_name, current_DID, (count_words / current_DID), numb_terms};
}

void ConcatenateBlocks(uint64_t count_block, const std::string& name) {

	uint64_t current_block = 1;
	uint64_t first_idt;
	uint64_t second_idt;
	std::string current_name(name + "1");

	while (current_block != count_block) {
		std::string first_name(current_name);
		uint8_t have_equivalent;
		std::string second_name(name + std::to_string(current_block + 1));
		std::ifstream first(first_name, std::ios::binary);
		current_name += std::to_string(count_block + 1);
		std::ofstream merge(current_name, std::ios::binary);
		uint64_t inform_term;
		char bait;

		while (first.get(bait)) {

			first_idt = static_cast<uint8_t>(bait);
			for (int i = 1; i < 8; ++i) {
				first.get(bait);
				first_idt += (static_cast<uint8_t>(bait) * pow(256, i));
			}

			have_equivalent = false;

			std::ifstream second(second_name, std::ios::binary);
			while (second.get(bait)) {

				second_idt = static_cast<uint8_t>(bait);
				for (int i = 1; i < 8; ++i) {
					second.get(bait);
					second_idt += (static_cast<uint8_t>(bait) * pow(256, i));
				}

				if (first_idt != second_idt) {

					inform_term = 0;
					for (int i = 0; i < 8; ++i) {
						second.get(bait);
						inform_term += (static_cast<uint8_t>(bait) * pow(256, i));
					}
					second.seekg(inform_term, std::ios::cur);

				} else {

					have_equivalent = true;
					std::unordered_map<uint64_t, std::set<uint64_t>> term;
					std::uint64_t DID;
					std::uint64_t DID_info;
					std::uint64_t line;
					uint64_t len_term = 0;
					char block;

					for (int i = 0; i < 8; ++i) {
						first.get(block);
						len_term += (static_cast<uint8_t>(block) * pow(256, i));
					}

					while (len_term != 0) {

						DID = 0;
						for (int i = 0; i < 8; ++i) {
							first.get(block);
							DID += (static_cast<uint8_t>(block) * pow(256, i));
						}

						DID_info = 0;
						for (int i = 0; i < 8; ++i) {
							first.get(block);
							DID_info += (static_cast<uint8_t>(block) * pow(256, i));
						}

						for (uint64_t i = 0; i < DID_info; i += 8) {
							line = 0;
							for (int j = 0; j < 8; ++j) {
								first.get(block);
								line += (static_cast<uint8_t>(block) * pow(256, j));
							}
							term[DID].insert(line);
						}
						len_term -= (16 + DID_info);
					}

					len_term = 0;
					for (int i = 0; i < 8; ++i) {
						second.get(block);
						len_term += (static_cast<uint8_t>(block) * pow(256, i));
					}

					while (len_term != 0) {

						DID = 0;
						for (int i = 0; i < 8; ++i) {
							second.get(block);
							DID += (static_cast<uint8_t>(block) * pow(256, i));
						}

						DID_info = 0;
						for (int i = 0; i < 8; ++i) {
							second.get(block);
							DID_info += (static_cast<uint8_t>(block) * pow(256, i));
						}

						for (uint64_t i = 0; i < DID_info; i += 8) {
							line = 0;
							for (int j = 0; j < 8; ++j) {
								second.get(block);
								line += (static_cast<uint8_t>(block) * pow(256, j));
							}
							term[DID].insert(line);
						}
						len_term -= (16 + DID_info);
					}

					for (int i = 0; i < 8; ++i) {
						merge.put(static_cast<char>(first_idt % 256));
						first_idt = (first_idt >> 8);
					}

					uint64_t all_size = 0;
					for (const auto& el: term) {
						all_size += (2 + el.second.size());
					}
					all_size *= 8;
					for (int i = 0; i < 8; ++i) {
						merge.put(static_cast<char>(all_size % 256));
						all_size = (all_size >> 8);
					}

					for (const auto& el: term) {

						uint64_t value = el.first;
						for (int i = 0; i < 8; ++i) {
							merge.put(static_cast<char>(value % 256));
							value = (value >> 8);
						}

						value = el.second.size() * 8;
						for (int i = 0; i < 8; ++i) {
							merge.put(static_cast<char>(value % 256));
							value = (value >> 8);
						}

						for (const auto& numb_line: el.second) {
							value = numb_line;
							for (int i = 0; i < 8; ++i) {
								merge.put(static_cast<char>(value % 256));
								value = (value >> 8);
							}
						}

					}

					break;
				}

			}
			second.close();

			if (!have_equivalent) {

				for (int i = 0; i < 8; ++i) {
					merge.put(static_cast<char>(first_idt % 256));
					first_idt = (first_idt >> 8);
				}

				uint64_t count_info = 0;
				char block;
				for (int i = 0; i < 8; ++i) {
					first.get(block);
					count_info += (static_cast<uint8_t>(block) * pow(256, i));
					merge.put(block);
				}

				count_info /= 8;
				for (uint64_t i = 0; i < count_info; ++i) {
					first.get(block);
					merge.put(block);
				}

			}

		}

		++current_block;
		first.close();
		merge.close();
		std::filesystem::remove(first_name);
		std::filesystem::remove(second_name);
	}

	std::filesystem::rename(current_name, name);

}