add_executable(main_index main_index.cpp)
add_executable(main_search main_search.cpp)

target_link_libraries(main_index PRIVATE console)
target_link_libraries(main_index PRIVATE revindex)
target_link_libraries(main_index PRIVATE search)

target_link_libraries(main_search PRIVATE console)
target_link_libraries(main_search PRIVATE revindex)
target_link_libraries(main_search PRIVATE search)
target_include_directories(main_search PUBLIC ${PROJECT_SOURCE_DIR})