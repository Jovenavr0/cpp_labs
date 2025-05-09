#include <iostream>
#include "ArgParser.h"

bool ArgumentParser::ArgParser::Parse(const std::vector<std::string>& vector) {
	bool answer = true;
	if (vector.empty()) {
		return false;
	}
	for (int i = 0; i < vector.size(); i++) {
		if (vector[i][0] == '-' && vector[i][1] == '-') {
			std::string now_name, now_value;
			int j = 1;
			while ((++j < vector[i].size()) && vector[i][j] != '=') {
				now_name.push_back(vector[i][j]);
			}
			while (++j < vector[i].size()) {
				now_value.push_back(vector[i][j]);
			}
			for (Argument* now_argument : all_argument_) {
				if (now_argument -> string_argument != nullptr) {
					if (now_argument -> string_argument -> name == now_name) {
						now_argument -> string_argument -> value = new std::string(now_value);
						if (now_argument -> string_argument -> store_value != nullptr) {
							*(now_argument -> string_argument -> store_value) = now_value;
						}
						break;
					}
				} else if (now_argument -> int_argument != nullptr) {
					if (now_argument -> int_argument -> name == now_name) {
						if (!now_argument -> int_argument -> multi_value) {
							now_argument -> int_argument -> value = new int(stoi(now_value));
						} else {
							now_argument -> int_argument -> value_vector -> push_back(stoi(now_value));
						}
						break;
					}
				} else if (now_argument -> flag_argument != nullptr) {
					if (now_argument -> flag_argument -> name == now_name) {
						*(now_argument -> flag_argument -> value) = true;
						break;
					}
				}
			}
		} else {
		if (vector[i][0] == '-' && vector[i][1] != '-') {
			std::string now_value;
			char now_abbreviation;
			int j = 0;
			if (vector[i].size() == 2 or vector[i][2] != '=') {
				while (++j < vector[i].size()) {
					for (Argument* now_argument: all_argument_) {
						if (now_argument -> flag_argument != nullptr) {
							if (now_argument -> flag_argument -> abbreviation == vector[i][j]) {
								*(now_argument -> flag_argument -> value) = true;
								break;
							}
						}
					}
				}
			} else {
				now_abbreviation = vector[i][++j];
				j++;
				while (++j < vector[i].size()) {
					now_value.push_back(vector[i][j]);
				}
				for (Argument* now_argument: all_argument_) {
					if (now_argument -> string_argument != nullptr) {
						if (now_argument -> string_argument -> abbreviation == now_abbreviation) {
							now_argument -> string_argument -> value = new std::string(now_value);
							if (now_argument -> string_argument -> store_value != nullptr) {
								*(now_argument -> string_argument -> store_value) = now_value;
							}
							break;
						}
					} else if (now_argument -> int_argument != nullptr) {
						if (now_argument -> int_argument -> abbreviation == now_abbreviation) {
							if (!now_argument -> int_argument -> multi_value) {
								now_argument -> int_argument -> value = new int(stoi(now_value));
							} else {
								now_argument -> int_argument -> value_vector->push_back(stoi(now_value));
							}
							break;
						}
					}
				}
			}
		}
		}
		 if (vector[i][0] != '-') {
			 for (Argument* now_argument: all_argument_) {
				 if (now_argument -> int_argument != nullptr) {
					 if (now_argument -> int_argument -> positional) {
						 while ((++i < vector.size()) && (vector[i][0] != '-')) {
							 now_argument -> int_argument -> value_vector -> push_back(stoi(vector[i]));
						 }
					 }
				 }
			 }
		 }
	}
	for (Argument* now_argument: all_argument_) {
		if (now_argument -> string_argument != nullptr) {
			if (now_argument -> string_argument -> value == nullptr) {
				answer = false;
				break;
			}
		} else if (now_argument -> int_argument != nullptr) {
			if (now_argument -> int_argument -> value == nullptr && !now_argument -> int_argument -> multi_value && !now_argument -> int_argument -> positional) {
				answer = false;
				break;
			}
			if (now_argument -> int_argument -> min_count && (now_argument -> int_argument -> value_vector -> size() < now_argument -> int_argument -> value_min_count)) {
				answer = false;
				break;
			}
		} else if (now_argument -> flag_argument != nullptr) {
			answer = true;
			break;
		}
	}

	return answer;
}

ArgumentParser::StringArgument& ArgumentParser::ArgParser::AddStringArgument(char abbreviation, std::string argument, std::string description) {
	auto* now_argument = new Argument;
	now_argument -> flag_argument = nullptr;
	now_argument -> int_argument = nullptr;
	now_argument -> string_argument = new StringArgument(argument, abbreviation, description, nullptr, nullptr);
	all_argument_.push_back(now_argument);

	return *now_argument -> string_argument;
}

ArgumentParser::StringArgument& ArgumentParser::ArgParser::AddStringArgument(std::string argument) {
	auto* now_argument = new Argument;
	now_argument -> flag_argument = nullptr;
	now_argument -> int_argument = nullptr;
	now_argument -> string_argument = new StringArgument(argument, argument[0], "", nullptr, nullptr);
	all_argument_.push_back(now_argument);

	return *now_argument -> string_argument;
}

ArgumentParser::IntArgument& ArgumentParser::ArgParser::AddIntArgument(char abbreviation, std::string argument) {
	auto* now_argument = new Argument;
	now_argument -> flag_argument = nullptr;
	now_argument -> int_argument = new IntArgument(argument, abbreviation, "", nullptr, false, false, false);
	now_argument -> string_argument = nullptr;
	all_argument_.push_back(now_argument);

	return *now_argument -> int_argument;
}

ArgumentParser::IntArgument& ArgumentParser::ArgParser::AddIntArgument(std::string argument, std::string description) {
	auto* now_argument = new Argument;
	now_argument -> flag_argument = nullptr;
	now_argument -> int_argument = new IntArgument(argument, argument[0], description, nullptr, false, false, false);
	now_argument -> string_argument = nullptr;
	all_argument_.push_back(now_argument);

	return *now_argument -> int_argument;
}

std::string ArgumentParser::ArgParser::GetStringValue(const std::string& name) {
	for (Argument* now_argument : all_argument_) {
		if (now_argument -> string_argument != nullptr) {
			if (now_argument -> string_argument -> name == name) {
				return *(now_argument -> string_argument -> value);
			}
		}
	}
}

int ArgumentParser::ArgParser::GetIntValue(const std::string& name) {
	for (Argument* now_argument : all_argument_) {
		if (now_argument -> int_argument != nullptr) {
			if (now_argument -> int_argument -> name == name) {
				return *(now_argument -> int_argument -> value);
			}
		}
	}
}

int ArgumentParser::ArgParser::GetIntValue(const std::string& name, int index) {
	for (Argument* now_argument : all_argument_) {
		if (now_argument -> int_argument != nullptr) {
			if (now_argument -> int_argument -> name == name) {
				return (*(now_argument -> int_argument -> value_vector))[index];
			}
		}

	}
}

bool ArgumentParser::ArgParser::GetFlag(const std::string& name) {
	for (Argument* now_argument : all_argument_) {
		if (now_argument -> flag_argument != nullptr) {
			if (now_argument -> flag_argument -> name == name) {
				return now_argument -> flag_argument -> value;
			}
		}
	}
}

std::string ArgumentParser::ArgParser::HelpDescription() {
	std::string help_info;
	std::string value_type;
	help_info += name_of_parser_;
	help_info.push_back('\n');
	for (const Argument* now_argument: all_argument_) {
		if (now_argument -> flag_argument != nullptr) {
			if (now_argument -> flag_argument -> name == "help") {
				help_info += now_argument -> flag_argument -> description;
				help_info.push_back('\n');
				break;
			}
		}
	}
	help_info.push_back('\n');
	for (const Argument* now_argument: all_argument_) {
		if (now_argument -> string_argument != nullptr) {
			help_info.push_back('-');
			help_info += now_argument -> string_argument -> abbreviation;
			help_info += ", --";
			help_info += now_argument -> string_argument -> name;
			value_type = "=<string>, ";
			help_info += value_type;
			help_info += now_argument -> string_argument -> description;
			help_info.push_back('\n');
		} else if (now_argument -> flag_argument != nullptr) {
			if (now_argument -> flag_argument -> name == "help") {
				continue;
			}
			help_info.push_back('-');
			help_info += now_argument -> flag_argument -> abbreviation;
			help_info += ", --";
			help_info += now_argument -> flag_argument -> name;
			help_info += ", ";
			help_info += now_argument -> flag_argument -> description;
			help_info.push_back('\n');
		} else if (now_argument -> int_argument != nullptr) {
			help_info += "--";
			help_info += now_argument -> int_argument -> name;
			value_type = "=<int>, ";
			help_info += value_type;
			help_info += now_argument -> int_argument -> description;
			help_info.push_back('\n');
		}
	}

	help_info.push_back('\n');

	std::cout << help_info;

	return help_info;
}

ArgumentParser::FlagArgument& ArgumentParser::ArgParser::AddFlag(char abbreviation, std::string flag, std::string description) {
	auto* now_argument = new Argument;
	bool* for_func = new bool(false);
	now_argument -> flag_argument = new FlagArgument(flag, abbreviation, for_func, for_func, description);
	now_argument -> int_argument = nullptr;
	now_argument -> string_argument = nullptr;
	all_argument_.push_back(now_argument);

	return *now_argument -> flag_argument;
}

ArgumentParser::FlagArgument& ArgumentParser::ArgParser::AddHelp(char abbreviation, std::string flag, std::string description) {
	auto* now_argument = new Argument;
	bool* for_func = new bool(false);
	now_argument -> flag_argument = new FlagArgument(flag, abbreviation, for_func, for_func, description);
	now_argument -> int_argument = nullptr;
	now_argument -> string_argument = nullptr;
	all_argument_.push_back(now_argument);

	return *now_argument -> flag_argument;
}

bool ArgumentParser::ArgParser::Help() {
	bool answer = false;
	for (Argument* now_argument: all_argument_) {
		if (now_argument -> flag_argument != nullptr) {
			if (now_argument -> flag_argument -> name == "help") {
				answer = true;
				break;
			}
		}
	}
	return answer;
}

void ArgumentParser::StringArgument::Default(std::string default_value) {
	value = &default_value;
}

void ArgumentParser::StringArgument::StoreValue(std::string& param) {
	store_value = &param;
}

ArgumentParser::StringArgument& ArgumentParser::StringArgument::MultiValue(size_t min_count) {
	this -> min_count = true;
	this -> value_min_count = min_count;
	return *this;
}

ArgumentParser::IntArgument& ArgumentParser::IntArgument::MultiValue() {
	this -> multi_value = true;
	return *this;
}

void ArgumentParser::IntArgument::StoreValues(std::vector<int>& vector) {
	this -> value_vector = &vector;
}

void ArgumentParser::FlagArgument::StoreValue(bool& value) {
	this -> value = &value;
}

ArgumentParser::IntArgument& ArgumentParser::IntArgument::MultiValue(size_t min_count) {
	this -> min_count = true;
	this -> value_min_count = min_count;
	return *this;
}

ArgumentParser::IntArgument& ArgumentParser::IntArgument::Positional() {
	this -> positional = true;
	return *this;
}

void ArgumentParser::FlagArgument::Default(bool default_value) {
	this -> value = &default_value;
}
