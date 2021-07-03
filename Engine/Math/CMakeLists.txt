add_library(Math
    Color.h
    Color.cpp
	Vectors.h
	Vectors.cpp
)

target_precompile_headers(Math
	PRIVATE 
	${COMMON_HEADERS}
)

target_include_directories(Math
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(Math
	DirectXMath
	Log
)