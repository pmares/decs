
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

add_executable( bdance ${SOURCES} )
target_link_libraries( bdance decs boinc boinc_api )

install( TARGETS bdance DESTINATION ${BIN_OUTPUT_PATH} )
# install( FILES README.txt RENAME README-BDANCE.txt DESTINATION ${DOC_OUTPUT_PATH} )
