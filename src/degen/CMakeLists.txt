project( degen )

set(SOURCES 
	degen.cpp
	nqdecs.cpp
)

add_executable( degen ${SOURCES} )

install( TARGETS degen DESTINATION ${BIN_OUTPUT_PATH} )
