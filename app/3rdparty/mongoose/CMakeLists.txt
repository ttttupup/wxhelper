cmake_minimum_required(VERSION 3.10...3.27)
aux_source_directory(. MONGOOSE_SOURCE)

add_library(mongoose ${MONGOOSE_SOURCE})

target_include_directories(mongoose INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})