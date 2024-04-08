cmake_minimum_required(VERSION 3.10...3.27)

# 设置项目名称和版本
project(detours VERSION 4.0.1 LANGUAGES C CXX)

MACRO(GET_WIN32_WINNT version)
    IF(WIN32 AND CMAKE_SYSTEM_VERSION)
		SET(ver ${CMAKE_SYSTEM_VERSION})
		STRING(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
		STRING(REGEX MATCH "^([0-9]+)" verMajor ${ver})
		# Check for Windows 10, b/c we'll need to convert to hex 'A'.
		IF("${verMajor}" MATCHES "10")
			SET(verMajor "A")
			STRING(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
		ENDIF("${verMajor}" MATCHES "10")
		# Remove all remaining '.' characters.
		STRING(REPLACE "." "" ver ${ver})
		# Prepend each digit with a zero.
		STRING(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
		SET(${version} "0x${ver}")
    ENDIF()
ENDMACRO()
  
# 根据配置设置DEBUG标志  
set(DETOURS_CONFIG ${CMAKE_BUILD_TYPE})  
if("${DETOURS_CONFIG}" STREQUAL "Debug")  
    set(DETOURS_DEBUG 1)  
else()  
    set(DETOURS_DEBUG 0)  
endif()  

####################################################

# 设置编译器标志 
set(CMAKE_CXX_FLAGS "/nologo /W4 /WX /we4777 /we4800 /Zi /MT /Gy /Gm- /Zl /Od /DDETOUR_DEBUG=${DETOURS_DEBUG}")
  

if(WIN32)
	IF(MSVC_VERSION GREATER_EQUAL 1700)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D DETOURS_CL_17_OR_NEWER")
	ENDIF()
	GET_WIN32_WINNT(ver)
	message(STATUS "Windows OS Version: ${ver}")
	if(ver EQUAL 0x0700)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D _USING_V110_SDK71_ /D DETOURS_WIN_7")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D _WIN32_WINNT=${ver}")
else()
    message(FATAL_ERROR "Only Win32 platforms are supported.")
endif()



if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "i386")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  /D _X86_ ")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "i686")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  /D _AMD64_ ")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "IA64")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  /D _IA64_ ")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  /D _X86_ ")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  /D _AMD64_ ")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "AMD64")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  /D _AMD64_ ")
else()
	message("Unknown architecture (x86, amd64, ia64, arm, arm64)")
endif()
	




set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D DETOURS_VERSION=0x4c0c1  /D WIN32_LEAN_AND_MEAN")


if(MSVC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D _CRT_SECURE_NO_WARNINGS  /EHsc")

else()
	# TODO: Shall we support gcc with MinGW, Cygwin or Clang based building system on Windows?
endif()

if(ENABLE_VERBOSELOG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D ENABLE_VERBOSELOG")
endif()

# 设置源文件和目标  


set(SOURCES  
	${CMAKE_CURRENT_SOURCE_DIR}/src/detours.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/src/modules.cpp    
	${CMAKE_CURRENT_SOURCE_DIR}/src/disasm.cpp     
	${CMAKE_CURRENT_SOURCE_DIR}/src/image.cpp      
	${CMAKE_CURRENT_SOURCE_DIR}/src/creatwth.cpp   
	${CMAKE_CURRENT_SOURCE_DIR}/src/disolx86.cpp   
	${CMAKE_CURRENT_SOURCE_DIR}/src/disolx64.cpp   
	${CMAKE_CURRENT_SOURCE_DIR}/src/disolia64.cpp  
	${CMAKE_CURRENT_SOURCE_DIR}/src/disolarm.cpp   
	${CMAKE_CURRENT_SOURCE_DIR}/src/disolarm64.cpp 
)  

# 添加库目标  
add_library(detours  ${SOURCES})  
target_include_directories(detours INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

