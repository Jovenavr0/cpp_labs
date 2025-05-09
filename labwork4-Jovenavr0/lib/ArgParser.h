#pragma once

#include <string>
#include <vector>

namespace ArgumentParser {

	struct StringArgument {
		StringArgument(std::string name, char abbreviation, std::string description, std::string* value, std::string* store_value): name(name), abbreviation(abbreviation), description(description), value(value), store_value(store_value)
		{}

		std::string name;
		std::string description;
		bool min_count;
		size_t value_min_count;
		char abbreviation;
		std::string* value;
		std::string* store_value;

		void Default(std::string default_value);
		void StoreValue(std::string& param);
		ArgumentParser::StringArgument& MultiValue(size_t min_count);
	};

	struct IntArgument {
		IntArgument(std::string name, char abbreviation, std::string description, int* value, bool multi_value, bool min_count, bool positional): name(name), abbreviation(abbreviation), description(description), value(value), multi_value(multi_value), min_count(min_count), positional(positional)
		{}

		std::string name;
		char abbreviation;
		int* value;
		bool multi_value;
		bool min_count;
		bool positional;
		size_t value_min_count;
		std::string description;
		std::vector<int>* value_vector;

		ArgumentParser::IntArgument& Positional();
		ArgumentParser::IntArgument& MultiValue();
		ArgumentParser::IntArgument& MultiValue(size_t min_count);
		void StoreValues(std::vector<int>& vector);
	};

	struct FlagArgument {
		FlagArgument(std::string name, char abbreviation, bool* value, bool store_value, std::string description): name(name), abbreviation(abbreviation), value(value), store_value(store_value), description(description)
		{}

		std::string name;
		std::string description;
		char abbreviation;
		bool* value;
		bool store_value;

		void Default(bool default_value);
		void StoreValue(bool& value);
	};

	struct Argument {
		IntArgument* int_argument;
		StringArgument* string_argument;
		FlagArgument* flag_argument;
	};

class ArgParser {
public:

	explicit ArgParser(const std::string& name): name_of_parser_(name)
	{}

	~ArgParser(){
		for(Argument* now_argument: all_argument_) {
			if (now_argument -> flag_argument != nullptr) {
				delete now_argument -> flag_argument;
			} else if (now_argument -> int_argument != nullptr) {
				delete now_argument -> int_argument;
			} else if (now_argument -> string_argument != nullptr) {
				delete now_argument -> string_argument;
			}
			delete now_argument;
		}
		all_argument_.clear();
	}

	bool Parse(const std::vector<std::string>& vector);

	bool Help();

	StringArgument& AddStringArgument(char abbreviation, std::string argument, std::string description="");
	StringArgument& AddStringArgument(std::string argument);

	IntArgument& AddIntArgument(std::string argument, std::string description="");
	IntArgument& AddIntArgument(char abbreviation, std::string argument);

	FlagArgument& AddFlag(char abbreviation, std::string flag, std::string description="");
	FlagArgument& AddHelp(char abbreviation, std::string flag, std::string description);

	std::string GetStringValue(const std::string& name);

	int GetIntValue(const std::string& name);
	int GetIntValue(const std::string& name, int index);

	bool GetFlag(const std::string& name);

	std::string HelpDescription();

private:
	std::string name_of_parser_;

	std::vector<Argument*> all_argument_;

};

} // namespace ArgumentParser