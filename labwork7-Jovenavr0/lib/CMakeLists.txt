add_library(parse parse.cpp parse.h)
add_library(config config.cpp config.h)
add_library(console console.cpp console.h)

target_link_libraries(console PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(config PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(parse PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(parse PRIVATE cpr::cpr)
