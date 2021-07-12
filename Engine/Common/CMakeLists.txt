add_library(Common
    Ref.h
	Time.h
    Common.cpp
)

target_include_directories(Common
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)
