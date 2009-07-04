project( libdecs )

set(SOURCES 
	dfileio.h
	dfileio.cpp
	dformat.h
	dformat.cpp
	dlx.h
	dlx.cpp
	sbcolumn.h
	sbcolumn.cpp
	sbmatrix.h
	sbmatrix.cpp
	sbnode.h
	sbnode.cpp
)

add_library( decs STATIC ${SOURCES} )
set_target_properties( decs PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

#install( TARGETS decs DESTINATION ${LIB_OUTPUT_PATH} )
