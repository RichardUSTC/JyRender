add_executable(Game 
	main.cpp
)

target_precompile_headers(Game
	PRIVATE
	${COMMON_HEADERS}
)

target_link_libraries(Game Engine)
