project( dance )

set(SOURCES 
	dance.cpp
)

include_directories( ${CMAKE_SOURCE_DIR}/decs )
#link_directories( ${CMAKE_BINARY_DIR}/decs )

add_executable( dance ${SOURCES} )
target_link_libraries( dance libdecs )

install( TARGETS dance DESTINATION ${BIN_OUTPUT_PATH} )
install( FILES README.txt RENAME README-DANCE.txt DESTINATION ${DOC_OUTPUT_PATH} )

add_subdirectory( examples )
