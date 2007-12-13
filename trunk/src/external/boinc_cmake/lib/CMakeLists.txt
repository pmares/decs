project( libboinc )

set(SOURCES 
	app_ipc.C
	diagnostics.C
	diagnostics_win.C
	filesys.C
	hostinfo.C
	md5.c
	md5_file.C
	mem_usage.C
	mfile.C
	miofile.C
	parse.C
	prefs.C
	proxy_info.C
	shmem.C
	stackwalker_win.cpp
	str_util.C
	util.C
	win_util.C
)

include_directories( 
	../
	../api
	../lib
#	../win_build
	../../boinc_samples/glut
	../../boinc_samples/image_libs
	../../boinc_samples/jpeglib
)

add_definitions( 
	-DBOINC_APP_GRAPHICS
	-DCLIENT
	-DHAVE_STD_MAX
	-DHAVE_STD_MIN
	-D_CONSOLE
	-D_MT
)

add_library( libboinc STATIC ${SOURCES} )

