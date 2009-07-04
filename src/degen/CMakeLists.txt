project( degen )

set(SOURCES 
	degen.cpp
	nqdecs.h
	nqdecs.cpp
)

include_directories( ${CMAKE_SOURCE_DIR}/decs )

add_executable( degen ${SOURCES} )
target_link_libraries( degen decs )

install( TARGETS degen DESTINATION ${BIN_OUTPUT_PATH} )
