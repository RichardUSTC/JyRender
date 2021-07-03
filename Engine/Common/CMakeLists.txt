add_library(Common
    Ref.h
	TimeUtils.h
    Common.cpp
)

target_precompile_headers(Common
	PRIVATE 
	${COMMON_HEADERS}
)

target_include_directories(Common
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(Common
	Log
)