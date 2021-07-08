set(HEADERS
)

set(SOURCES
	Engine.cpp
)

source_group(Engine FILES ${HEADERS} ${SOURCES})

add_library(Engine
	${HEADERS}
	${SOURCES}
)

add_subdirectory(Common)
add_subdirectory(Application)

target_precompile_headers(Engine
	PRIVATE ${COMMON_HEADERS}
)

target_link_libraries(Engine
	Common
	Application
)

target_include_directories(Engine
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)