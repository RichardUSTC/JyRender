add_library(Engine
	Engine.cpp
)

add_subdirectory(Common)
add_subdirectory(Application)
add_subdirectory(Log)

target_link_libraries(Engine
	Common
	Application
	Log
)

target_include_directories(Engine
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)