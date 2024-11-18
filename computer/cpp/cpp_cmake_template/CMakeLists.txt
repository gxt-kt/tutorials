cmake_minimum_required(VERSION 3.0)
project(cmake_template)

include(cmake/help.cmake)

set(CMAKE_BUILD_TYPE Release) # Debug Release RelWithDebInfo
# set a debug postfix
set(CMAKE_DEBUG_POSTFIX "-dbg")

# set(CMAKE_C_COMPILER "gcc") # gcc clang
# set(CMAKE_CXX_COMPILER "g++") # g++ clang++

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set the most strict compile rule
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall)
    # add_compile_options(-Wall -Wextra -Wpedantic -Werror)
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /WX)
endif()

include_directories(include)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    message("Building for Linux platform")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    message("Building for macOS platform")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    message("Building for Windows platform")
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()


option(OPTION_TEST "whether or not to build the tests" ON)
if(OPTION_TEST)
    PrintVariable(OPTION_TEST)
endif()


# add_definitions(-DMY_MACRO)
# target_compile_definitions(demo PRIVATE MY_MACRO)

# add_library(demo SHARED ${detail_header} ${header} ${src})
# target_include_directories(demo)
# target_compile_definitions(demo PUBLIC COMPILE_TEST=)

add_executable(demo src/demo.cc)
