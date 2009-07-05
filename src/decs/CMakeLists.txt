
set(SOURCES 
	decs.pb.h
	decs.pb.cc
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

include_directories(${CMAKE_SOURCE_DIR}/vendor/protobuf/include)
add_library( decs STATIC ${SOURCES} )
set_target_properties( decs PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
