#include <iostream>
#include <lib/parser.h>
#include <lib/scattering.h>

int main(int argc, char** argv) {

    Options element = ParseArguments(argc, argv);
    ParseSize size = GetSizes(element.input_directory);
	MainPointField field(size);
	field = FillField(element, field);
	MakeScattering(field);

    return EXIT_SUCCESS;
}
