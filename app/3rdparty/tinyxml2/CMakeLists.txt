cmake_minimum_required(VERSION 3.10...3.27)
aux_source_directory(. TINYXML2_SOURCE)


set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

#add_library(tinyxml2 tinyxml2.cpp tinyxml2.h)


add_library(tinyxml2 ${TINYXML2_SOURCE})
add_library(tinyxml2::tinyxml2 ALIAS tinyxml2)


target_include_directories(tinyxml2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})