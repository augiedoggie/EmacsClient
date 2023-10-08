cmake_minimum_required(VERSION 3.19)

project(EmacsClient)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules")

include(UseHaiku)

option(STRICT_WARNINGS "Compile with extra warnings and errors" ON)
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 2.96 AND STRICT_WARNINGS)
	add_compile_options(-Wall -Wextra -Wshadow -Werror)
endif()

haiku_add_executable(
	EmacsClient
	EmacsClient.cpp
	EmacsClient.rdef
)

target_link_libraries(EmacsClient be)
