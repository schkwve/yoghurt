# Unity test framework
add_subdirectory(unity)

add_executable(
	test_main
	test_main.c
)

target_link_libraries(
	test_main
	unity
	yoghurt_core
)

target_include_directories(
	test_main PUBLIC
	${PROJECT_SOURCE_DIR}/tests/unity
)

set_target_properties(
	test_main
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_PATH}"
)

add_test(test_main "${TEST_OUTPUT_PATH}/test_main")

