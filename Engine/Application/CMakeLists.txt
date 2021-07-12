add_library(Application
    Application.h
    Application.cpp
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
