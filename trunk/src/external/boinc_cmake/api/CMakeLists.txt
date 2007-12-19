project( libboincapi )

set(SOURCES 
	boinc_api.C
	graphics_api.C
	graphics_data.C
	graphics_impl.C
	gutil.C
	gutil_text.C
	reduce_lib.C
	reduce_main.C
	texfont.C
	texture.C
	tgalib.C
	txf_util.C
	windows_opengl.C
)

include_directories( 
	../
#	../api
	../lib
	../win_build
	../../boinc_samples/win_build
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

add_library( libboincapi STATIC ${SOURCES} )

