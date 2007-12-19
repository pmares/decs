project( degen )

set( degen_SRCS
	degen.cpp
)

add_executable( degen ${degen_SRCS} )
target_link_libraries( degen decs nqdecs )

install( TARGETS degen DESTINATION ${BIN_OUTPUT_PATH} )
