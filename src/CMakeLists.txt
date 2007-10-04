project(decs)

# Set version number
set( DECS_VERSION_MAJOR 0 )
set( DECS_VERSION_MINOR 2 )
set( DECS_VERSION_RELEASE 0 )
set( DECS_VERSION "${DECS_VERSION_MAJOR}.${DECS_VERSION_MINOR}.${DECS_VERSION_RELEASE}" )
set( DECS_VERSION_STRING "${DECS_VERSION} (DECS Beta)" )

# Define the generic version of the libraries here
# This makes it easy to advance when the next release comes
set( GENERIC_LIB_VERSION "${DECS_VERSION}" )
set( GENERIC_LIB_SOVERSION "${DECS_VERSION_MAJOR}" )

# Always include srcdir and builddir in include path
# This saves typing ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY} in about every subdir
# since cmake 2.4.0
set( CMAKE_INCLUDE_CURRENT_DIR ON )

# Put the include dirs which are in the source or build tree
# before all other include dirs, so the headers in the sources
# are prefered over the already installed ones
# since cmake 2.4.1
set( CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON )

# Add -s to omit symbol information in the release version
# This should reduce their size quite a bit
set( CMAKE_EXE_LINKER_FLAGS_RELEASE -s )
set( CMAKE_SHARED_LINKER_FLAGS_RELEASE -s )
set( CMAKE_MODULE_LINKER_FLAGS_RELEASE -s )

if(WIN32)
	set( BIN_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX} )
	set( LIB_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX} )
	set( DATA_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX} )
else(WIN32)
	set( BIN_OUTPUT_PATH bin )
	set( LIB_OUTPUT_PATH lib )
	set( DATA_OUTPUT_PATH share/doc )
endif(WIN32)


# List the subdirectories
add_subdirectory( dance )
add_subdirectory( dlx )
add_subdirectory( degen )
add_subdirectory( nqdecs )

install( FILES COPYING.txt DESTINATION ${DATA_OUTPUT_PATH} )
