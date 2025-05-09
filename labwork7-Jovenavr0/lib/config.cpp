#include "config.h"

json parse_config(){
	std::ifstream file(R"(C:\Users\vvkar\CLionProjects\sem_2\labwork7-Jovenavr0\package.json)"); // Реальный путь вместо относительного
	json town_list = json::parse(file);
	file.close();

	return town_list;
}