include(OpenGLES)

set(GLAD_PATH
	${CMAKE_SOURCE_DIR}/External/glad
)

include_directories(${GLAD_PATH}/include)

if(NOT TARGET glad)
	add_library(glad ${GLAD_PATH}/src/glad.c)
	target_link_libraries(glad
		PUBLIC GLESv2
		PUBLIC EGL
	)

	target_include_directories(glad INTERFACE ${GLAD_PATH}/include)
endif()