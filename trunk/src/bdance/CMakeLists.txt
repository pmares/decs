project( bdance )

set( SOURCES
	bdance.cpp
)

set(BOINC_DIR ${CMAKE_SOURCE_DIR}/boinc/boinc)

include_directories( 
	${CMAKE_SOURCE_DIR}/decs
	${BOINC_DIR}
	${BOINC_DIR}/lib
	${BOINC_DIR}/api
)
link_directories(
	${CMAKE_BINARY_DIR}/decs
)

add_executable( bdance ${SOURCES} )
target_link_libraries( bdance libdecs libboinc libboincapi )

install( TARGETS bdance DESTINATION ${BIN_OUTPUT_PATH} )
# install( FILES README.txt RENAME README-BDANCE.txt DESTINATION ${DOC_OUTPUT_PATH} )
