project( boinc )


set(BOINC_DIR ${CMAKE_SOURCE_DIR}/boinc/boinc)
set(BOINC_SAMPLE_DIR ${CMAKE_SOURCE_DIR}/boinc/boinc_samples)
set(BOINCAPI_DIR ${BOINC_DIR}/api)
set(BOINCLIB_DIR ${BOINC_DIR}/lib)


set(boincapi_SOURCES 
	${BOINCAPI_DIR}/boinc_api.C
	${BOINCAPI_DIR}/graphics_api.C
	${BOINCAPI_DIR}/graphics_data.C
	${BOINCAPI_DIR}/graphics_impl.C
	${BOINCAPI_DIR}/gutil.C
	${BOINCAPI_DIR}/gutil_text.C
	${BOINCAPI_DIR}/reduce_lib.C
	${BOINCAPI_DIR}/reduce_main.C
	${BOINCAPI_DIR}/texfont.C
	${BOINCAPI_DIR}/texture.C
	${BOINCAPI_DIR}/tgalib.C
	${BOINCAPI_DIR}/txf_util.C
	${BOINCAPI_DIR}/windows_opengl.C
)

include_directories( 
	${BOINC_DIR}
#	${BOINCAPI_DIR}
	${BOINCLIB_DIR}
	${BOINC_DIR}/win_build
	${BOINC_SAMPLE_DIR}/win_build
	${BOINC_SAMPLE_DIR}/glut
	${BOINC_SAMPLE_DIR}/image_libs
	${BOINC_SAMPLE_DIR}/jpeglib
)

add_definitions( 
	-DBOINC_APP_GRAPHICS
	-DCLIENT
	-DHAVE_STD_MAX
	-DHAVE_STD_MIN
	-D_CONSOLE
	-D_MT
)

add_library( libboincapi STATIC ${boincapi_SOURCES} )



set(boinclib_SOURCES 
	${BOINCLIB_DIR}/app_ipc.C
	${BOINCLIB_DIR}/diagnostics.C
	${BOINCLIB_DIR}/diagnostics_win.C
	${BOINCLIB_DIR}/filesys.C
	${BOINCLIB_DIR}/hostinfo.C
	${BOINCLIB_DIR}/md5.c
	${BOINCLIB_DIR}/md5_file.C
	${BOINCLIB_DIR}/mem_usage.C
	${BOINCLIB_DIR}/mfile.C
	${BOINCLIB_DIR}/miofile.C
	${BOINCLIB_DIR}/parse.C
	${BOINCLIB_DIR}/prefs.C
	${BOINCLIB_DIR}/proxy_info.C
	${BOINCLIB_DIR}/shmem.C
	${BOINCLIB_DIR}/stackwalker_win.cpp
	${BOINCLIB_DIR}/str_util.C
	${BOINCLIB_DIR}/util.C
	${BOINCLIB_DIR}/win_util.C
)

include_directories( 
	${BOINC_DIR}
	${BOINCAPI_DIR}
	${BOINCLIB_DIR}
#	${BOINC_DIR}/win_build
	${BOINC_SAMPLE_DIR}/glut
	${BOINC_SAMPLE_DIR}/image_libs
	${BOINC_SAMPLE_DIR}/jpeglib
)

add_definitions( 
	-DBOINC_APP_GRAPHICS
	-DCLIENT
	-DHAVE_STD_MAX
	-DHAVE_STD_MIN
	-D_CONSOLE
	-D_MT
)

add_library( libboinc STATIC ${boinclib_SOURCES} )

