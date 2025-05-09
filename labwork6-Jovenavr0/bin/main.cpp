#include "lib/archivator.h"
#include "lib/parser.h"
#include <iostream>


int main(int argc, char* argv[]){

	std::vector <Arguments> all_arguments = Parse(argc, argv);

	Actions(all_arguments);

	return EXIT_SUCCESS;
}
