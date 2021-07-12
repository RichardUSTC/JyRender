include(FindGit)
find_package(Git)

if(NOT Git_FOUND)
    message(FATAL_ERROR "Git not found")
endif()

include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.8.5
    GIT_SHALLOW    ON
    GIT_PROGRESS   ON
)

FetchContent_MakeAvailable(spdlog)
