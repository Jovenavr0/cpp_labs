#include <deque>
#include "search.h"

const double b = 0.75;
const double k = 1.6;

int64_t Search_world(const std::string& word, const std::string& filename, uint64_t TID_count) {
	std::ifstream terms(filename, std::ios::binary);

	std::string current_terms;
	uint64_t len_info;
	uint64_t counter = 0;
	int64_t pointer;
	char block;
	char symb;

	while ((current_terms != word) && (counter < TID_count)) {
		++counter;
		len_info = 0;
		for (int i = 0; i < 8; ++i) {
			terms.get(block);
			len_info += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, i));
		}

		current_terms.clear();
		for (uint64_t i = 0; i < len_info; ++i) {
			terms.get(symb);
			current_terms += symb;
		}

		pointer = 0;
		for (int i = 0; i < 8; ++i) {
			terms.get(block);
			pointer += static_cast<int64_t>(static_cast<uint8_t>(block) * pow(256, i));
		}

	}

	terms.close();

	if (current_terms == word) {
		return pointer;
	}

	return -1;
}

void Score_world(const std::string& word, const names& naming, std::unordered_map<uint64_t, double>& all_files) {

	double score;
	double IDF;
	uint64_t DID;
	uint64_t DID_info;
	uint64_t line;
	std::unordered_map<uint64_t, std::vector<uint64_t>> current_term;
	int64_t pointer = Search_world(word, naming.terms, naming.count_TID);
	if (pointer == -1) {
		return;
	}

	std::ifstream file(naming.index, std::ios::binary);
	file.seekg(pointer, std::ios::beg);

	uint64_t len_term = 0;
	uint64_t id_term = 0;
	char block;

	for (int i = 0; i < 8; ++i) {
		file.get(block);
		id_term += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, i));
	}

	for (int i = 0; i < 8; ++i) {
		file.get(block);
		len_term += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, i));
	}

	while (len_term != 0) {
		DID = 0;
		for (int i = 0; i < 8; ++i) {
			file.get(block);
			DID += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, i));
		}
		DID_info = 0;
		for (int i = 0; i < 8; ++i) {
			file.get(block);
			DID_info += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, i));
		}

		for (uint64_t i = 0; i < DID_info; i += 8) {
			line = 0;
			for (int j = 0; j < 8; ++j) {
				file.get(block);
				line += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, j));
			}
			current_term[DID].push_back(line);
		}

		len_term -= (16 + DID_info);
	}

	IDF = std::log((static_cast<double>(naming.count) - current_term.size() + 0.5) / (static_cast<double>(current_term.size()) + 0.5) + 1);

	for (const auto& el: current_term) {

		if (!all_files.contains(el.first)) {
			continue;
		}

		std::ifstream DID_file(naming.files, std::ios::binary);
		uint64_t current_DID = 0;
		uint64_t size_DID;
		uint64_t symbol_len;

		while (el.first != current_DID) {

			current_DID = 0;
			for (int i = 0; i < 8; ++i) {
				DID_file.get(block);
				current_DID += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, i));
			}

			size_DID = 0;
			for (int i = 0; i < 8; ++i) {
				DID_file.get(block);
				size_DID += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, i));
			}

			for (int i = 0; i < size_DID; ++i) {
				DID_file.get(block);
			}

			symbol_len = 0;
			for (int i = 0; i < 8; ++i) {
				DID_file.get(block);
				symbol_len += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, i));
			}

		}

		DID_file.close();

		score = ((static_cast<double>(el.second.size()) * (k + 1)) / (static_cast<double>(el.second.size()) + k * (1 - b - b * static_cast<double>(symbol_len) / static_cast<double>(naming.avgdl))));

		all_files[el.first] += IDF * score;

	}

}

void Search(int argc, char** argv, const names& naming) {

	std::deque<uint64_t> open;
	std::vector<std::string> terms_request;
	std::deque<uint64_t> close;
	std::string first;
	std::string operation;
	std::string second;
	uint64_t start;
	uint64_t prev_start;
	uint64_t prev_finish;
	uint64_t DID;
	uint64_t DID_info;
	std::vector<std::pair<double, uint64_t>> answer;
	std::unordered_map<uint64_t, uint64_t> first_term;
	std::unordered_map<uint64_t, uint64_t> second_term;
	std::unordered_map<uint64_t, uint64_t> third_term;
	uint64_t finish;
	std::unordered_map<uint64_t, double> all_files_score;


	for (uint64_t i = 1; i < argc; ++i) {
		std::string elem(argv[i]);

		if (close.size() > open.size()) {
			std::cerr << "Place the brackets correctly in your query";
			exit(EXIT_FAILURE);
		}

		if (elem[0] == '(') {
			open.push_back(i);
		}

		if (elem[elem.size() - 1] == ')') {
			close.push_back(i);
		}
	}

	open.push_front(1);
	close.push_back(argc - 1);

	if (open.size() != close.size()) {
		std::cerr << "The number of opening brackets must be equal to the closing one. Please enter a valid query";
		exit(EXIT_FAILURE);
	}

	// для самых глубоких скобок
	start = open.back();
	prev_start = start;
	open.pop_back();
	finish = close.front();
	close.pop_front();
	prev_finish = finish;

	first = argv[start];
	while (first[0] == '(') {
		first = first.substr(1);
	}
	while (first[first.size() - 1] == ')') {
		first = first.substr(0, first.size() - 1);
	}
	terms_request.push_back(first);
	int64_t pointer = Search_world(first, naming.terms, naming.count_TID);

	if (pointer != -1) {

		std::ifstream file(naming.index, std::ios::binary);
		file.seekg(pointer, std::ios::beg);

		uint64_t len_term = 0;
		uint64_t id_term = 0;
		char block;

		for (int i = 0; i < 8; ++i) {
			file.get(block);
			id_term += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, i));
		}

		for (int i = 0; i < 8; ++i) {
			file.get(block);
			len_term += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, i));
		}

		while (len_term != 0) {
			DID = 0;
			for (int i = 0; i < 8; ++i) {
				file.get(block);
				DID += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, i));
			}
			first_term[DID] = DID;

			DID_info = 0;
			for (int i = 0; i < 8; ++i) {
				file.get(block);
				DID_info += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, i));
			}

			file.seekg(DID_info, std::ios::cur);

			len_term -= (16 + DID_info);
		}

		file.close();

	}

	for (uint64_t i = (start + 1); i <= finish; ++i) {

		if (operation.empty()) {
			operation = argv[i];
			if (operation != "AND" && operation != "OR") {
				std::cerr << "incorrect query (use only parentheses, OR, AND)";
				exit(EXIT_FAILURE);
			}
			continue;
		}

		second = argv[i];
		while (second[second.size() - 1] == ')') {
			second = second.substr(0, second.size() - 1);
		}
		terms_request.push_back(second);
		int64_t pointer2 = Search_world(second, naming.terms, naming.count_TID);

		if (pointer2 != -1) {

			std::ifstream file(naming.index, std::ios::binary);
			file.seekg(pointer2, std::ios::beg);

			uint64_t len_term = 0;
			char block;

			uint64_t id_term = 0;
			for (int j = 0; j < 8; ++j) {
				file.get(block);
				id_term += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, j));
			}

			for (int j = 0; j < 8; ++j) {
				file.get(block);
				len_term += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, j));
			}

			while (len_term != 0) {
				DID = 0;
				for (int j = 0; j < 8; ++j) {
					file.get(block);
					DID += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, j));
				}
				second_term[DID] = DID;
				DID_info = 0;
				for (int j = 0; j < 8; ++j) {
					file.get(block);
					DID_info += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, j));
				}

				file.seekg(DID_info, std::ios::cur);

				len_term -= (16 + DID_info);
			}

			file.close();

			if (operation == "AND") {
				for (auto el: first_term) {
					if (second_term.contains(el.first) && !third_term.contains(el.first)) {
						third_term[el.first] = el.first;
					}
				}
			} else {
				for (auto el: first_term) {
					if (!third_term.contains(el.first)) {
						third_term[el.first] = el.first;
					}
				}
				for (auto el: second_term) {
					if (!third_term.contains(el.first)) {
						third_term[el.first] = el.first;
					}
				}
			}

			first_term.clear();
			second_term.clear();
			first_term = third_term;
			third_term.clear();
		}

		operation.clear();
		second.clear();

	}

	// для вложений
	while (!open.empty()) {
		start = open.back();
		open.pop_back();
		finish = close.front();
		close.pop_front();

		for (uint64_t i = (prev_start - 1); i >= start; --i) {

			if (operation.empty()) {
				operation = argv[i];
				if (operation != "AND" && operation != "OR") {
					std::cerr << "incorrect query (use only parentheses, OR, AND)";
					exit(EXIT_FAILURE);
				}
				continue;
			}

			second = argv[i];
			while (second[0] == '(') {
				second = second.substr(1);
			}
			terms_request.push_back(second);
			int64_t pointer2 = Search_world(second, naming.terms, naming.count_TID);

			if (pointer2 != -1) {

				std::ifstream file(naming.index, std::ios::binary);
				file.seekg(pointer2, std::ios::beg);

				uint64_t len_term = 0;
				char block;

				uint64_t id_term = 0;
				for (int j = 0; j < 8; ++j) {
					file.get(block);
					id_term += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, j));
				}

				for (int j = 0; j < 8; ++j) {
					file.get(block);
					len_term += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, j));
				}

				while (len_term != 0) {
					DID = 0;
					for (int j = 0; j < 8; ++j) {
						file.get(block);
						DID += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, j));
					}
					second_term[DID] = DID;
					DID_info = 0;
					for (int j = 0; j < 8; ++j) {
						file.get(block);
						DID_info += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, j));
					}

					file.seekg(DID_info, std::ios::cur);

					len_term -= (16 + DID_info);
				}

				file.close();

				if (operation == "AND") {
					for (auto el: first_term) {
						if (second_term.contains(el.first) && !third_term.contains(el.first)) {
							third_term[el.first] = el.first;
						}
					}
				} else {
					for (auto el: first_term) {
						if (!third_term.contains(el.first)) {
							third_term[el.first] = el.first;
						}
					}
					for (auto el: second_term) {
						if (!third_term.contains(el.first)) {
							third_term[el.first] = el.first;
						}
					}
				}

				first_term.clear();
				second_term.clear();
				first_term = third_term;
			}

			operation.clear();
			second.clear();

		}

		for (uint64_t i = (prev_finish + 1); i <= finish; ++i) {

			if (operation.empty()) {
				operation = argv[i];
				if (operation != "AND" && operation != "OR") {
					std::cerr << "incorrect query (use only parentheses, OR, AND)";
					exit(EXIT_FAILURE);
				}
				continue;
			}

			second = argv[i];
			while (second[second.size() - 1] == ')') {
				second = second.substr(0, second.size() - 1);
			}
			terms_request.push_back(second);
			int64_t pointer2 = Search_world(second, naming.terms, naming.count_TID);

			if (pointer2 != -1) {

				std::ifstream file(naming.index, std::ios::binary);
				file.seekg(pointer2, std::ios::beg);

				uint64_t len_term = 0;
				char block;

				uint64_t id_term = 0;
				for (int j = 0; j < 8; ++j) {
					file.get(block);
					id_term += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, j));
				}

				for (int j = 0; j < 8; ++j) {
					file.get(block);
					len_term += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, j));
				}

				while (len_term != 0) {
					DID = 0;
					for (int j = 0; j < 8; ++j) {
						file.get(block);
						DID += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, j));
					}
					second_term[DID] = DID;
					DID_info = 0;
					for (int j = 0; j < 8; ++j) {
						file.get(block);
						DID_info += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, j));
					}

					file.seekg(DID_info, std::ios::cur);

					len_term -= (16 + DID_info);
				}

				file.close();

				if (operation == "AND") {
					for (auto el: first_term) {
						if (second_term.contains(el.first) && !third_term.contains(el.first)) {
							third_term[el.first] = el.first;
						}
					}
				} else {
					for (auto el: first_term) {
						if (!third_term.contains(el.first)) {
							third_term[el.first] = el.first;
						}
					}
					for (auto el: second_term) {
						if (!third_term.contains(el.first)) {
							third_term[el.first] = el.first;
						}
					}
				}

				first_term.clear();
				second_term.clear();
				first_term = third_term;
			}

			operation.clear();
			second.clear();

		}

		prev_start = start;
		prev_finish = finish;

	}

	for (auto& el: first_term) {
		all_files_score[el.first] = 0;
	}

	for (const auto& el: terms_request) {
		Score_world(el, naming, all_files_score);
	}

	for (auto& el: all_files_score) {
		answer.emplace_back(el.second, el.first);
	}

	std::sort(answer.rbegin(), answer.rend());

	uint16_t limiter;

	if (answer.size() < 10) {
		limiter = answer.size();
	} else {
		limiter = 10;
	}

	for (uint16_t j = 0; j < limiter; ++j) {

		std::ifstream DID_file(naming.files, std::ios::binary);
		std::string filename;
		uint64_t current_DID = 0;
		uint64_t size_DID;
		uint64_t symbol_len;
		char block;

		while (answer[j].second != current_DID) {

			current_DID = 0;
			for (int i = 0; i < 8; ++i) {
				DID_file.get(block);
				current_DID += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, i));
			}

			size_DID = 0;
			for (int i = 0; i < 8; ++i) {
				DID_file.get(block);
				size_DID += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, i));
			}

			filename.clear();
			for (int i = 0; i < size_DID; ++i) {
				DID_file.get(block);
				filename += block;
			}

			symbol_len = 0;
			for (int i = 0; i < 8; ++i) {
				DID_file.get(block);
				symbol_len += static_cast<uint64_t>(static_cast<uint8_t>(block) * pow(256, i));
			}

		}

		DID_file.close();

		std::cout << j + 1 << ") " <<  filename << '\n';

	}

	if (limiter == 0) {
		std::cout << "None of the files in the collection are suitable for the request.";
	}

}