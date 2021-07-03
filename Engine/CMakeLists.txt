add_library(Engine
	Engine.h
	Engine.cpp
)

add_subdirectory(Common)
add_subdirectory(Application)
add_subdirectory(Log)
add_subdirectory(Device)
add_subdirectory(Math)

target_link_libraries(Engine
	Common
	Application
	Log
	Device
	Math
)

target_include_directories(Engine
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)