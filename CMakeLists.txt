cmake_minimum_required(VERSION 3.0)

project(kinova_tools)
set(TOOLS_HOME ./.)


set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Define command line options.
add_definitions(-DMODE=${MODE})
add_definitions(-DUNICODE)

# Choose architecture on UNIX
if (UNIX)
    add_definitions(-DARCH=${ARCH}) 
endif (UNIX)

# Build only x86 on Windows
if (WIN32)
    set(ARCH "x86")
endif (WIN32)

# Set build-specific configurations.
if(MODE STREQUAL "debug")
    set(CMAKE_BUILD_TYPE Debug)
    if (${ARCH} STREQUAL "x86")
    	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TOOLS_HOME}/bin/build/x86/Debug)
    else()
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TOOLS_HOME}/bin/build/x64/Debug)
    endif()
elseif(MODE STREQUAL "release")
    set(CMAKE_BUILD_TYPE Release)
    if (${ARCH} STREQUAL "x86")
    	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TOOLS_HOME}/bin/build/x86/Release)
    else()
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TOOLS_HOME}/bin/build/x64/Release)
    endif()
endif()

include_directories(${TOOLS_HOME}/includes)

add_executable(kinova_tools ${TOOLS_HOME}/src/main.cpp)

if (UNIX)
    target_link_libraries(kinova_tools ${CMAKE_DL_LIBS})
endif (UNIX)
