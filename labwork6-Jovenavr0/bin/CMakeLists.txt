add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE archivator)
target_link_libraries(${PROJECT_NAME} PRIVATE parser)
target_link_libraries(${PROJECT_NAME} PRIVATE hamming)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})