cmake_minimum_required(VERSION 3.0.0)
project(ConsoleApplication VERSION 1.0.0)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D '_UNICODE' /D 'UNICODE'")

file(GLOB INJECT_CPP_FILES  ${PROJECT_SOURCE_DIR}/*.cc  ${PROJECT_SOURCE_DIR}/*.cpp)

add_executable (ConsoleApplication  ${INJECT_CPP_FILES})

SET_TARGET_PROPERTIES(ConsoleApplication PROPERTIES LINKER_LANGUAGE C
                      ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
                      LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
                      RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
                      OUTPUT_NAME "ConsoleApplication"
                      PREFIX "")
           