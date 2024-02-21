cmake_minimum_required(VERSION 3.28)
project(yoghurt C)
include(CTest)

# output directories
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/modules")
set(TEST_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/tests")

# include directory for modules
include_directories("${PROJECT_SOURCE_DIR}/include")

# modules
add_subdirectory(modules)

# server
add_subdirectory(server)

# BUILD_TESTING is ON by default
if (BUILD_TESTING)
	enable_testing()
	add_subdirectory(tests)
endif()

