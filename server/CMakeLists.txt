add_executable(
	yoghurt_server
	main.c
	mod/module.c
	cjson/cjson.c
)

include_directories(
	.
)

