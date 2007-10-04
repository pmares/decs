project( nqdecs )

set( nqdecs_SRCS
	nqdecs.cpp
)

add_library( nqdecs SHARED ${nqdecs_SRCS} )
set_target_properties( nqdecs PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install( TARGETS nqdecs DESTINATION ${LIB_OUTPUT_PATH} )
