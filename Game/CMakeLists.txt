set(HEADERS
)

set(SOURCES
	main.cpp
)
source_group(Game FILES ${HEADERS} ${SOURCES})

add_executable(Game 
	${HEADERS}
	${SOURCES}
)

target_precompile_headers(Game
	PRIVATE ${COMMON_HEADERS}
)

target_link_libraries(Game Engine)
