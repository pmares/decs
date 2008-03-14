project(decs)

# Define CMake options for DECS
if(UNIX)
	OPTION( DECS_PACKAGE_FHS "Package files according to the File Hierarchy Standard (FHS) used by Unix distributions" ON)
endif(UNIX)
OPTION( DECS_BUILD_BOINC "Build the bdance BOINC application together with the required BOINC dependencies" NO)
OPTION( DECS_BUILD_UTILS "Build the DECS utility programs: dance and degen and any required dependencies" YES)

# Set version number
set( DECS_VERSION_MAJOR 0 )
set( DECS_VERSION_MINOR 3 )
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

# Standard directory variables, most of which are unused
# See the GNU make documentation and the File Hierarchy Standard for
# more information about standard directory variables
# http://www.gnu.org/software/make/manual/make.html#Directory-Variables
# http://www.pathname.com/fhs/

if(DECS_PACKAGE_FHS)
	set( BIN_OUTPUT_PATH bin )
	set( LIB_OUTPUT_PATH lib )
	set( DATAROOT_OUTPUT_PATH share )
	set( DATA_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}/decs-dance )
	set( DOC_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}/doc/decs-dance )
endif(DECS_PACKAGE_FHS)

set( INCLUDE_OUTPUT_PATH include )
set( INFO_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}info )
set( MAN_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}man )
set( MAN1_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}man1 )
set( MAN2_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}man2 )
set( MAN3_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}man3 )
set( MAN4_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}man4 )
set( MAN5_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}man5 )
set( MAN6_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}man6 )
set( MAN7_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}man7 )
set( MAN8_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}man8 )
set( MAN9_OUTPUT_PATH ${DATAROOT_OUTPUT_PATH}man9 )


# CPack configuration
INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_NAME "decs-dance")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "DECS dance solves exact cover problems using the Dancing Links algorithm")
SET(CPACK_PACKAGE_VENDOR "Jan Magne Tjensvold")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/dance/README.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR ${DECS_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${DECS_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${DECS_VERSION_RELEASE})
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "decs-dance ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")

IF(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
#  SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\dance.exe")
  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} decs-dance")
  SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\decs.googlecode.com")
  SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\decs.googlecode.com")
#  SET(CPACK_NSIS_CONTACT "me@my-personal-home-page.com")
  SET(CPACK_NSIS_MODIFY_PATH ON)
ELSE(WIN32 AND NOT UNIX)
#  SET(CPACK_STRIP_FILES "bin/dance")
#  SET(CPACK_SOURCE_STRIP_FILES "")
ENDIF(WIN32 AND NOT UNIX)

SET(CPACK_GENERATOR "TGZ;TZ")
SET(CPACK_PACKAGE_EXECUTABLES "decs-dance-${DECS_VERSION}" "DECS dance")
SET(CPACK_SOURCE_GENERATOR "TGZ;TZ")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "decs-dance-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_SOURCE_IGNORE_FILES
	"^${PROJECT_SOURCE_DIR}.*/svn./"
	"^${PROJECT_SOURCE_DIR}/build/"
	"^${PROJECT_SOURCE_DIR}/boinc/boinc/"
	"^${PROJECT_SOURCE_DIR}/boinc/boinc_samples/"
)
INCLUDE(CPack)


# List the subdirectories
add_subdirectory( decs )
if(DECS_BUILD_UTILS)
	add_subdirectory( dance )
	add_subdirectory( degen )
endif(DECS_BUILD_UTILS)

if(DECS_BUILD_BOINC)
	add_subdirectory( bdance )
	add_subdirectory( boinc )
endif(DECS_BUILD_BOINC)

install( FILES COPYING.txt DESTINATION ${DOC_OUTPUT_PATH} )
