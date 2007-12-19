project( degen )

set( degen_SRCS
	degen.cpp
)

include_directories( ${CMAKE_SOURCE_DIR}/nqdecs )
#link_directories( ${CMAKE_BINARY_DIR}/nqdecs )

add_executable( degen ${degen_SRCS} )
target_link_libraries( degen nqdecs )

install( TARGETS degen DESTINATION ${BIN_OUTPUT_PATH} )
