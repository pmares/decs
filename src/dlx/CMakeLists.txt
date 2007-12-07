project( dlx )

set( dlx_SRCS
	column.cpp
	dlx.cpp
	node.cpp
	util.cpp
)

add_library( dlx STATIC ${dlx_SRCS} )
set_target_properties( dlx PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install( TARGETS dlx DESTINATION ${LIB_OUTPUT_PATH} )
install( FILES README.txt RENAME README-DLX.txt DESTINATION ${DOC_OUTPUT_PATH})
