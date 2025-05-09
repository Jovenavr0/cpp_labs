#include "console.h"

void output_to_console(std::string& info_text, json& config, std::string& town_name) {

	int count_days;
	int mas_index = 0;
	json weather_info = json::parse(info_text);

	count_days = weather_info["hourly"]["time"].size() / 24;

	if (count_days == 1){
		std::cout << "Прогноз погоды на сегодня\n";
	} else if (count_days == 3) {
		std::cout << "Прогноз погоды на 3 дня\n";
	} else if (count_days == 7) {
		std::cout << "Прогноз погоды на 7 дней\n";
	} else if (count_days == 14) {
		std::cout << "Прогноз погоды на 14 дней\n";
	} else if (count_days == 16) {
		std::cout << "Прогноз погоды на 16 дней\n";
	}

	std::string date = std::string(weather_info["hourly"]["time"][0]);

	std::cout << '\n' << town_name << '\n' << "Дата: " << date.substr(0, date.size() - 6) << '\n';

	std::cout << '\n' << "Погода сейчас:" << '\n';

	std::cout << config["config"]["weather_codes"][0][std::to_string((int)(weather_info["current"]["weather_code"]))] << '\n';
	std::cout << "Температура: " << weather_info["current"]["temperature_2m"] << " ℃" << '\n';
	std::cout << "Ощущается как: " << weather_info["current"]["apparent_temperature"] << " ℃" << '\n';
	std::cout << "Скорость ветра: " << weather_info["current"]["wind_speed_10m"] << " км\\ч" << "\n";
	std::cout << "Вероятность выпадения осадков: " << weather_info["current"]["precipitation_probability"] << "%" << '\n';


	while (count_days) {

		mas_index += 5; // Почему не цикл  for
		date = std::string(weather_info["hourly"]["time"][mas_index]);
		std::cout << '\n' << "Дата: " << date.substr(0, date.size() - 6) << '\n';

		std::cout << "\n" <<  "Утром: " << "\n";

		std::cout << config["config"]["weather_codes"][0][std::to_string((int)(weather_info["hourly"]["weather_code"][mas_index]))] << '\n';
		std::cout << "Температура: " << weather_info["hourly"]["temperature_2m"][mas_index] << " ℃" << '\n';
		std::cout << "Ощущается как: " << weather_info["hourly"]["apparent_temperature"][mas_index] << " ℃" << '\n';
		std::cout << "Скорость ветра: " << weather_info["hourly"]["wind_speed_10m"][mas_index] << " км\\ч" << "\n";
		std::cout << "Вероятность выпадения осадков: " << weather_info["hourly"]["precipitation_probability"][mas_index] << "%" << '\n';

		std::cout << "\n" << "Днем: " << "\n";

		mas_index += 6;
		std::cout << config["config"]["weather_codes"][0][std::to_string((int)(weather_info["hourly"]["weather_code"][mas_index]))] << '\n';
		std::cout << "Температура: " << weather_info["hourly"]["temperature_2m"][mas_index] << " ℃" << '\n';
		std::cout << "Ощущается как: " << weather_info["hourly"]["apparent_temperature"][mas_index] << " ℃" << '\n';
		std::cout << "Скорость ветра: " << weather_info["hourly"]["wind_speed_10m"][mas_index] << " км\\ч" << "\n";
		std::cout << "Вероятность выпадения осадков: " << weather_info["hourly"]["precipitation_probability"][mas_index] << "%" << '\n';

		std::cout << "\n" << "Вечером: " << "\n";

		mas_index += 6;
		std::cout << config["config"]["weather_codes"][0][std::to_string((int)(weather_info["hourly"]["weather_code"][mas_index]))] << '\n';
		std::cout << "Температура: " << weather_info["hourly"]["temperature_2m"][mas_index] << " ℃" << '\n';
		std::cout << "Ощущается как: " << weather_info["hourly"]["apparent_temperature"][mas_index] << " ℃" << '\n';
		std::cout << "Скорость ветра: " << weather_info["hourly"]["wind_speed_10m"][mas_index] << " км\\ч" << "\n";
		std::cout << "Вероятность выпадения осадков: " << weather_info["hourly"]["precipitation_probability"][mas_index] << "%" << '\n';

		std::cout << "\n" << "Ночью: " << "\n";

		mas_index += 6;
		std::cout << config["config"]["weather_codes"][0][std::to_string((int)(weather_info["hourly"]["weather_code"][mas_index]))] << '\n';
		std::cout << "Температура: " << weather_info["hourly"]["temperature_2m"][mas_index] << " ℃" << '\n';
		std::cout << "Ощущается как: " << weather_info["hourly"]["apparent_temperature"][mas_index] << " ℃" << '\n';
		std::cout << "Скорость ветра: " << weather_info["hourly"]["wind_speed_10m"][mas_index] << " км\\ч" << "\n";
		std::cout << "Вероятность выпадения осадков: " << weather_info["hourly"]["precipitation_probability"][mas_index] << "%" << '\n';

		count_days -= 1;

	}



}