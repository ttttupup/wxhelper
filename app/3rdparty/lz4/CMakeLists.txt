cmake_minimum_required(VERSION 3.10...3.27)
aux_source_directory(. LZ4_SOURCE)

add_library(lz4 ${LZ4_SOURCE})

target_include_directories(lz4 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})