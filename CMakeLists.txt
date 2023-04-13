cmake_minimum_required(VERSION 3.0.0)
project(wxhelper VERSION 1.0.0)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D '_UNICODE' /D 'UNICODE'")

file(GLOB CPP_FILES  ${PROJECT_SOURCE_DIR}/src/*.cc  ${PROJECT_SOURCE_DIR}/src/*.cpp)



include_directories(c:/soft/vcpkg/installed/x86-windows/include)

# add_subdirectory(3rd)
# add_subdirectory(source)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-mongoose CONFIG REQUIRED)
# find_package(spdlog CONFIG REQUIRED)
# find_package(minhook CONFIG REQUIRED)


add_library(wxhelper  SHARED ${CPP_FILES} )


#target_include_directories(wxhelper PUBLIC 3rd/mongoose )

target_link_libraries(wxhelper PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(wxhelper PRIVATE unofficial::mongoose::mongoose)
# target_link_libraries(wxhelper PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
# target_link_libraries(wxhelper PRIVATE minhook::minhook)

SET_TARGET_PROPERTIES(wxhelper PROPERTIES LINKER_LANGUAGE C
                      ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
                      LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
                      RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
                      OUTPUT_NAME "wxhelper"
                      PREFIX "")
           