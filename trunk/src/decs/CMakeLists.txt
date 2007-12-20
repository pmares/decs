project( libdecs )

set(SOURCES 
	dfileio.cpp
	dformat.cpp
	dlx.cpp
	sbcolumn.cpp
	sbmatrix.cpp
	sbnode.cpp
)

add_library( decs STATIC ${SOURCES} )
set_target_properties( decs PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

#install( TARGETS decs DESTINATION ${LIB_OUTPUT_PATH} )
