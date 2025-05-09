#include <iostream>
#include <chrono>
#include "../lib/console.h"

int main(int argc, char** argv) {
	auto start = std::chrono::high_resolution_clock::now();

	RunSearch(argc, argv);

	auto end = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	return EXIT_SUCCESS;
}

