project( boinc )


set(BOINC_DIR boinc)
set(BOINC_SAMPLE_DIR boinc_samples)
set(BOINC_API_DIR ${BOINC_DIR}/api)
set(BOINC_LIB_DIR ${BOINC_DIR}/lib)



include_directories( 
	${BOINC_DIR}
	${BOINC_DIR}/client
	${BOINC_DIR}/openssl/include
	${BOINC_API_DIR}
	${BOINC_LIB_DIR}
)

if(UNIX)
	if(APPLE)
		include_directories( 
			${BOINC_LIB_DIR}/mac
		)
	endif(APPLE)
else(UNIX)
	if(WIN32)
		include_directories(
			${BOINC_DIR}/win_build
			${BOINC_SAMPLE_DIR}/glut
			${BOINC_SAMPLE_DIR}/image_libs
			${BOINC_SAMPLE_DIR}/jpeglib
			${BOINC_SAMPLE_DIR}/win_build
		)
	endif(WIN32)
endif(UNIX)

add_definitions( 
	-DBOINC_APP_GRAPHICS
	-DHAVE_STD_MAX
	-DHAVE_STD_MIN
	-D_CONSOLE
	-D_MT
)




# BOINC API

set(BOINC_API_SOURCES 
	${BOINC_API_DIR}/boinc_api.C
	${BOINC_API_DIR}/graphics2_util.C
)

if(UNIX)
	LIST( APPEND BOINC_API_SOURCES
		${BOINC_LIB_DIR}/diagnostics.C
		${BOINC_LIB_DIR}/shmem.C
	)
	link_libraries(boinc_api pthread)
endif(UNIX)

add_library( boinc_api STATIC ${BOINC_API_SOURCES} )




# BOINC Graphics API

set(BOINC_GRAPHICS_API_SOURCES 
	${BOINC_API_DIR}/graphics_api.C
	${BOINC_API_DIR}/graphics_data.C
	${BOINC_API_DIR}/graphics_impl.C
	${BOINC_API_DIR}/gutil.C
	${BOINC_API_DIR}/gutil_text.C
	${BOINC_API_DIR}/reduce_lib.C
	${BOINC_API_DIR}/reduce_main.C
	${BOINC_API_DIR}/texfont.C
	${BOINC_API_DIR}/texture.C
#	${BOINC_API_DIR}/tgalib.C  # Maybe Win only?
	${BOINC_API_DIR}/txf_util.C	
)

if(UNIX)
	LIST( APPEND BOINC_GRAPHICS_API_SOURCES 
		${BOINC_API_DIR}/x_opengl.C
	)
else(UNIX)
	if(WIN32)
		LIST( APPEND BOINC_GRAPHICS_API_SOURCES 
			${BOINC_API_DIR}/windows_opengl.C
		)
	endif(WIN32)
endif(UNIX)

add_library( boinc_graphics_api STATIC ${BOINC_GRAPHICS_API_SOURCES} )




# BOINC Graphics Library

set(BOINC_GRAPHICS_LIB_SOURCES 
	${BOINC_API_DIR}/graphics_lib.C
	${BOINC_API_DIR}/graphics_data.C
	${BOINC_API_DIR}/reduce_main.C
)

add_library( boinc_graphics_lib STATIC ${BOINC_GRAPHICS_LIB_SOURCES} )





# BOINC Graphics Implementation

set(BOINC_GRAPHICS_IMPL_SOURCES 
	${BOINC_API_DIR}/graphics_data.C
	${BOINC_API_DIR}/graphics_impl.C
	${BOINC_API_DIR}/graphics_impl_lib.C
	${BOINC_API_DIR}/gutil.C
	${BOINC_API_DIR}/gutil_text.C
	${BOINC_API_DIR}/reduce_lib.C
	${BOINC_API_DIR}/texfont.C
	${BOINC_API_DIR}/texture.C
	${BOINC_API_DIR}/txf_util.C	
)

if(UNIX)
	LIST( APPEND BOINC_GRAPHICS_IMPL_SOURCES 
		${BOINC_API_DIR}/x_opengl.C
	)
else(UNIX)
	if(WIN32)
		LIST( APPEND BOINC_GRAPHICS_IMPL_SOURCES 
			${BOINC_API_DIR}/windows_opengl.C
		)
	endif(WIN32)
endif(UNIX)

add_library( boinc_graphics_impl STATIC ${BOINC_GRAPHICS_IMPL_SOURCES} )




# BOINC Graphics 2 Library

set(BOINC_GRAPHICS2_SOURCES 
	${BOINC_API_DIR}/gutil.C
	${BOINC_API_DIR}/gutil_text.C
	${BOINC_API_DIR}/reduce_lib.C
	${BOINC_API_DIR}/texfont.C
	${BOINC_API_DIR}/texture.C
	${BOINC_API_DIR}/txf_util.C
	${BOINC_API_DIR}/graphics2.C
)

if(UNIX)
	LIST( APPEND BOINC_GRAPHICS2_SOURCES 
		${BOINC_API_DIR}/graphics2_unix.C
	)
endif(UNIX)

add_library( boinc_graphics2 STATIC ${BOINC_GRAPHICS2_SOURCES} )




# BOINC Library

set(BOINCLIB_SOURCES 
	${BOINC_LIB_DIR}/app_ipc.C
	${BOINC_LIB_DIR}/base64.C
	${BOINC_LIB_DIR}/crypt.C
	${BOINC_LIB_DIR}/diagnostics.C
	${BOINC_LIB_DIR}/filesys.C
#	${BOINC_LIB_DIR}/gui_rpc_client.C
#	${BOINC_LIB_DIR}/gui_rpc_client_ops.C
#	${BOINC_LIB_DIR}/gui_rpc_client_print.C
	${BOINC_LIB_DIR}/hostinfo.C
	${BOINC_LIB_DIR}/md5.c
	${BOINC_LIB_DIR}/md5_file.C
	${BOINC_LIB_DIR}/mem_usage.C
	${BOINC_LIB_DIR}/mfile.C
	${BOINC_LIB_DIR}/miofile.C
	${BOINC_LIB_DIR}/msg_log.C
	${BOINC_LIB_DIR}/network.C
	${BOINC_LIB_DIR}/parse.C
	${BOINC_LIB_DIR}/prefs.C
	${BOINC_LIB_DIR}/proxy_info.C
	${BOINC_LIB_DIR}/shmem.C
	${BOINC_LIB_DIR}/str_util.C
	${BOINC_LIB_DIR}/util.C
)

if(UNIX)
	LIST( APPEND BOINCLIB_SOURCES 
		${BOINC_LIB_DIR}/procinfo_unix.C
	)
else(UNIX)
	if(WIN32)
		LIST( APPEND BOINCLIB_SOURCES 
			${BOINC_LIB_DIR}/diagnostics_win.C
			${BOINC_LIB_DIR}/stackwalker_win.cpp
			${BOINC_LIB_DIR}/win_util.C
		)
	endif(WIN32)
endif(UNIX)


add_library( boinc STATIC ${BOINCLIB_SOURCES} )

