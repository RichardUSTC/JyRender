set(HEADERS
    Ref.h
	Time.h
)

set(SOURCES
    Common.cpp
)

source_group(Common FILES ${HEADERS} ${SOURCES})

add_library(Common
	${HEADERS}
	${SOURCES}
)

target_precompile_headers(Common
	PRIVATE ${COMMON_HEADERS}
)
