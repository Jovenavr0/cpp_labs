#include "parse.h"

std::pair <float, float> get_coordinate(std::string now_town){
	std::pair <float, float> coord;
	std::string json_string;
	std::string api_url = "https://api.api-ninjas.com/v1/city?name=" + now_town;

	cpr::Url url = cpr::Url{api_url};
	cpr::Header header = cpr::Header{{"X-Api-Key", "secret_token ;)"}};
	cpr::Response r = cpr::Get(url, header);

	json_string = r.text.substr(1, r.text.size() - 2);

	json Doc = json::parse(json_string);
	coord.first = Doc["latitude"];
	coord.second = Doc["longitude"];

	return coord;
}

std::string get_weather(std::pair <float, float> now_coordinate, int count_days){
	std::string url_api_open_meteo = "https://api.open-meteo.com/v1/forecast";

	cpr::Url url_api = cpr::Url{url_api_open_meteo};
	cpr::Parameters param = cpr::Parameters{{"latitude", std::to_string(now_coordinate.first)},
											{"longitude", std::to_string(now_coordinate.second)},
											{"forecast_days", std::to_string(count_days)},
											{"current", "weather_code"},
											{"current", "temperature_2m"},
											{"current", "apparent_temperature"},
											{"current", "wind_speed_10m"},
											{"current", "precipitation_probability"},
											{"hourly", "weather_code"},
											{"hourly", "temperature_2m"},
											{"hourly", "apparent_temperature"},
											{"hourly", "precipitation_probability"},
											{"hourly", "precipitation"},
											{"hourly", "wind_speed_10m"},
											{"hourly", "surface_pressure"},
											{"hourly", "rain"},
											{"hourly", "snowfall"},
											{"hourly", "snow_depth"}};

	cpr::Response r = cpr::Get(url_api, param);

	return r.text;
}