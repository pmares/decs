project(decs)

# Define CMake options for DECS
OPTION( BUILD_BOINC "Build the bdance BOINC application together with the required BOINC dependencies" NO)
OPTION( BUILD_UTILS "Build the DECS utility programs: dance and degen and any required dependencies" YES)

# Set version number
set( DECS_VERSION_MAJOR 0 )
set( DECS_VERSION_MINOR 3 )
set( DECS_VERSION_RELEASE 0 )
#set( DECS_REVISION  )
set( DECS_VERSION "${DECS_VERSION_MAJOR}.${DECS_VERSION_MINOR}.${DECS_VERSION_RELEASE}" )
#set( DECS_VERSION_REV "${DECS_VERSION_MAJOR}.${DECS_VERSION_MINOR}.${DECS_VERSION_RELEASE}-r${DECS_REVISION}" )
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

# Standard directory variables, most of which are unused
# See the GNU make documentation and the File Hierarchy Standard for
# more information about standard directory variables
# http://www.gnu.org/software/make/manual/make.html#Directory-Variables
# http://www.pathname.com/fhs/

set( BIN_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX}/bin )
set( LIB_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX}/lib )
set( INCLUDE_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX}/include )
set( DATAROOT_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX}/share )
set( DATA_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}/decs )
set( DOC_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}/doc/decs-${DECS_VERSION} )
set( INFO_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}/info )
set( MAN_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}/man )
set( MAN1_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}/man1 )
set( MAN2_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}/man2 )
set( MAN3_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}/man3 )
set( MAN4_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}/man4 )
set( MAN5_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}/man5 )
set( MAN6_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}/man6 )
set( MAN7_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}/man7 )
set( MAN8_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}/man8 )
set( MAN9_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}/man9 )

if(WIN32)
	set( BIN_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX} )
	set( LIB_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX} )
	set( DATAROOT_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX} )
	set( DATA_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX} )
	set( DOC_OUTPUT_PATH ${DATA_OUTPUT_PATH} )
	# INFO and MAN paths are not supported in Windows
endif(WIN32)

# List the subdirectories
add_subdirectory( decs )
if(BUILD_UTILS)
	add_subdirectory( dance )
	add_subdirectory( degen )
endif(BUILD_UTILS)

if(BUILD_BOINC)
	add_subdirectory( bdance )
	add_subdirectory( boinc )
endif(BUILD_BOINC)

install( FILES COPYING.txt DESTINATION ${DOC_OUTPUT_PATH} )
