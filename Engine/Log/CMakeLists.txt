add_library(Log
	Log.h
	Log.cpp
)

target_precompile_headers(Log
	PRIVATE 
	${COMMON_HEADERS}
)

target_include_directories(Log
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(Log
	spdlog::spdlog
)
