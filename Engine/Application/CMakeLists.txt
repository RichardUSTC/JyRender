set(HEADERS
    Application.h
)

set(SOURCES
    Application.cpp
)

source_group(Application FILES ${HEADERS} ${SOURCES})

add_library(Application
	${HEADERS}
	${SOURCES}
)

target_precompile_headers(Application
	PRIVATE ${COMMON_HEADERS}
)
