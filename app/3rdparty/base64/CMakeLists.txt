cmake_minimum_required(VERSION 3.10...3.27)
aux_source_directory(. BASE64_SOURCE)

add_library(base64 ${BASE64_SOURCE})

target_include_directories(base64 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})