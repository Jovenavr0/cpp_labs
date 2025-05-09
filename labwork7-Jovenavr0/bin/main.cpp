#include <iostream>
#include <conio.h>
#include <limits>
#include <windows.h>
#include "lib/parse.h"
#include "lib/config.h"
#include "lib/console.h"

int main(){

	SetConsoleOutputCP(CP_UTF8);

	int town_index = 0;
	long long start_time;
	int number_of_options_days;
	int number_of_options_town; //  Добавить в одну структуру
	int frequency_update;
	int index = 0;
	int count_days;
	char press_key;
	std::pair <float, float> now_coordinate;
	json info_config;
	std::string now_town;
	std::string weather_info;

	info_config = parse_config();

	now_town = info_config["config"]["list_of_cities"][town_index];
	count_days = info_config["config"]["count_days"][index];
	number_of_options_days = info_config["config"]["count_days"].size();
	number_of_options_town = info_config["config"]["count_cities"];
	frequency_update = info_config["config"]["frequency"];

	now_coordinate = get_coordinate(now_town);
	weather_info = get_weather(now_coordinate, count_days);

	output_to_console(weather_info, info_config, now_town);
	start_time = time(nullptr);

	while (true) {
		if (char press_key = getch()) {


			if (press_key == VK_ESCAPE) {
				return 0;
			}
			if (press_key == 'n') {
				if (town_index == (number_of_options_town - 1)) {
					town_index = 0;
				} else {
					town_index += 1;
				}

				now_town = info_config["config"]["list_of_cities"][town_index]; // Можно вынести в отдельную функцию
				now_coordinate = get_coordinate(now_town);
				weather_info = get_weather(now_coordinate, count_days);

				output_to_console(weather_info, info_config, now_town);
				start_time = time(nullptr); // Вынести вниз

			}
			if (press_key == 'p') {
				if (town_index == 0) {
					town_index = number_of_options_town - 1;
				} else {
					town_index -= 1;
				}

				now_town = info_config["config"]["list_of_cities"][town_index];
				now_coordinate = get_coordinate(now_town);
				weather_info = get_weather(now_coordinate, count_days);

				std::cout << '\n' << now_town << '\n';

				output_to_console(weather_info, info_config, now_town);
				start_time = time(nullptr);

			}
			if (press_key == '+') {
				if (index == (number_of_options_days - 1)) {
					index = 0;
				} else {
					index += 1;
				}

				count_days = info_config["config"]["count_days"][index];
				weather_info = get_weather(now_coordinate, count_days);

				output_to_console(weather_info, info_config, now_town);
				start_time = time(nullptr);

			}
			if (press_key == '-') {
				if (index == 0) {
					index = (number_of_options_days - 1); // Вывод всех дней в чистую
				} else {
					index -= 1;
				}

				count_days = info_config["config"]["count_days"][index];
				weather_info = get_weather(now_coordinate, count_days);

				output_to_console(weather_info, info_config, now_town);
				start_time = time(nullptr); // НЕТ ОЧИСТКИ КОНСОЛИ

			}

			while(kbhit()){
				getch();
			}


		} else if (time(nullptr) - start_time >= frequency_update ) {
			weather_info = get_weather(now_coordinate, count_days);
			output_to_console(weather_info, info_config, now_town);
			while(kbhit()){
				getch();
			}
			start_time = time(nullptr);
		}
	}

	return 0;
}