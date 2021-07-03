cmake_minimum_required(VERSION 3.16)

project(Game)

set(CMAKE_UNITY_BUILD ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# enable warning as error
if(MSVC)
    # add_compile_options(/W4 /WX)
    add_compile_options(/W4)
    set(CMAKE_STATIC_LINKER_FLAGS
        "${CMAKE_STATIC_LINKER_FLAGS} /NODEFAULTLIB")
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake)

set(ENGINE_MAJOR_VERSION 0)
set(ENGINE_MINOR_VERSION 1)

set(COMMON_HEADERS
    <vector>
    <string>
    <memory>
    <cstdint>
    <functional>
    <type_traits>
    <Log/Log.h>
)

include(ExternalLibraries)

add_subdirectory(Engine)
add_subdirectory(Game)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Game)
