if(WIN32)
	set(PLATFORM_RELATED_SOURCE
		Win32/WindowWin32.cpp
	)
endif()

add_library(Application
    Application.h
    Application.cpp
	Window.h
	Window.cpp
	${PLATFORM_RELATED_SOURCE}
)

target_precompile_headers(Application
	PRIVATE 
	${COMMON_HEADERS}
)

target_include_directories(Application
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(Application
	Common
	Log
)
