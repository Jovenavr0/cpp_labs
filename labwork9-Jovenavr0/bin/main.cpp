#include "lib/adapters.h"
#include <iostream>
#include <map>

int main () {

	std::vector<char> s{'h', 'e', 'l', 'l', 'o'};
	std::map<std::string, int> check{{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5} };
	std::cout << check.begin() -> first;
	std::vector<int> f{1, 2, 3, 4, 5, 6};
	f.push_back(7);
	auto op = [](unsigned char c) -> unsigned char { return std::toupper(c); };

	for (auto& el: s | adapters::transformation(op)) {
		std::cout << el << ' ';
	}

	std::cout << '\n';

	for (auto& el: s | adapters::take(2)) {
		std::cout << el << ' ';
	}

	std::cout << '\n';

	for (auto& el: s | adapters::drop(2)) {
		std::cout << el << ' ';
	}

	for (auto& el: f | adapters::filter([](int elem) {return elem % 2 == 0;}) | adapters::transformation([](int elem) {return elem * elem;} )) {
		std::cout << el << ' ';
	}

	std::cout << '\n';

	for (auto& el: s | adapters::reverse()) {
		std::cout << el << ' ';
	}

	std::cout << '\n';

	for (auto& el: f | adapters::drop(2) | adapters::filter([](int a) -> bool {return a % 2 == 0; })) {
		std::cout << el << ' ';
	}

	std::cout << '\n';

	for (auto& el:  check | adapters::keys()) {
		std::cout << el << ' ';
	}

	std::cout << '\n';

	for (auto& el: check | adapters::values()) {
		std::cout << el << ' ';
	}

	return 0;
}