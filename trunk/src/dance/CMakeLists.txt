project( dance )

set( dance_SRCS
	dance.cpp
)

include_directories( ${CMAKE_SOURCE_DIR}/dlx )
link_directories( ${CMAKE_BINARY_DIR}/dlx )

add_executable( dance ${dance_SRCS} )
target_link_libraries( dance dlx )

install( TARGETS dance DESTINATION ${BIN_OUTPUT_PATH} )
install( FILES README.txt RENAME README-DANCE.txt DESTINATION ${DOC_OUTPUT_PATH} )

add_subdirectory( examples )
