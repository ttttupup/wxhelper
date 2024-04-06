cmake_minimum_required(VERSION 3.10...3.27)
aux_source_directory(./src BASE_SOURCE)
aux_source_directory(./src/include BASE_HEADER_SOURCE)

add_library(base ${BASE_SOURCE} ${BASE_HEADER_SOURCE})

target_include_directories(base PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/src/include)



