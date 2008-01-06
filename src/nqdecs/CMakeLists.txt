project( nqdecs )

set(SOURCES 
	nqdecs.cpp
)

add_library( nqdecs STATIC ${SOURCES} )
set_target_properties( nqdecs PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

#install( TARGETS nqdecs DESTINATION ${LIB_OUTPUT_PATH} )
