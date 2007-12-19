project( bdance )

set( SOURCES
	bdance.cpp
)

include_directories( 
	${CMAKE_SOURCE_DIR}/decs
	${CMAKE_SOURCE_DIR}/external/boinc
	${CMAKE_SOURCE_DIR}/external/boinc/lib
	${CMAKE_SOURCE_DIR}/external/boinc/api
)
link_directories(
	${CMAKE_BINARY_DIR}/decs
)

add_executable( bdance ${SOURCES} )
target_link_libraries( bdance libdecs libboinc libboincapi )

install( TARGETS bdance DESTINATION ${BIN_OUTPUT_PATH} )
# install( FILES README.txt RENAME README-BDANCE.txt DESTINATION ${DOC_OUTPUT_PATH} )
