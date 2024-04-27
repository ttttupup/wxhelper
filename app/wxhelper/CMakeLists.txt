cmake_minimum_required(VERSION 3.10...3.27)
project(wxhelper VERSION 1.0.0)
# enable_language(ASM_MASM)
# add_compile_options("/experimental:module")

message(${CMAKE_VERSION})

option(USE_EXTERNAL_JSON "Use an external JSON library" OFF)
option(USE_EXTERNAL_DETOURS "Use an external detours library" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}    /std:c++17  /MD  /EHsc /FAcs")


file(GLOB CPP_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c )

file(GLOB ASM_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.asm )

if(USE_EXTERNAL_DETOURS)
    include_directories(${VCPKG_INSTALLED_DIR}/x64-windows/include ${DETOURS_INCLUDE_DIRS}) 
else()
    include_directories(${VCPKG_INSTALLED_DIR}/x64-windows/include ) 
endif()
add_subdirectory(../3rdparty 3rdparty)

add_subdirectory(../base base)
   
add_library(wxhelper  SHARED ${CPP_FILES} ${ASM_FILES})

target_compile_definitions(wxhelper PRIVATE WECHAT_VERSION=391019)

# target_include_directories(wxhelper 
#     PRIVATE     ../base/src/include
#     PRIVATE     ../3rdparty/spdlog/include  
#     PRIVATE     ../3rdparty/base64        
#     PRIVATE     ../3rdparty/lz4        
#     PRIVATE     ../3rdparty/mongoose
#     PRIVATE     ../3rdparty/tinyxml2
#     PRIVATE     ../3rdparty/Detours/src
# )

target_link_libraries(wxhelper 
    PRIVATE base
    PRIVATE spdlog::spdlog
    PRIVATE lz4
    PRIVATE base64
    PRIVATE mongoose
    PRIVATE nlohmann_json::nlohmann_json
    PRIVATE tinyxml2
    PRIVATE detours
)

SET_TARGET_PROPERTIES(wxhelper PROPERTIES LINKER_LANGUAGE C
                      ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
                      LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
                      RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
                      OUTPUT_NAME "wxhelper"
                      PREFIX "")
           
