if(WIN32)
	set(PLATFORM_RELATED_SOURCE
		GLES/GLESDevice.h
		GLES/GLESDevice.cpp
		GLES/GLESDeviceBuffer.h
		GLES/GLESDeviceBuffer.cpp
		GLES/GLESDeviceShader.h
		GLES/GLESDeviceShader.cpp
	)
endif()

add_library(Device
    Device.h
    Device.cpp
	DeviceBuffer.h
	DeviceBuffer.cpp
	DeviceShader.h
	DeviceShader.cpp
	${PLATFORM_RELATED_SOURCE}
)

target_precompile_headers(Device
	PRIVATE 
	${COMMON_HEADERS}
)

target_include_directories(Device
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(Device
	Common
	Log
	GLESv2
	EGL
)
