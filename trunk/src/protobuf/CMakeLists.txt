

set(PROTOBUF_DIR src)

include( CheckIncludeFiles )
include( CheckFunctionExists )
include( FindThreads )
include( FindZLIB )

# We only need libprotobuf so we can disable zlib and not compile protoc.

# Checking for hash_map and hash_set is tricky so we just say they don't exist for the moment.

if (NOT HASH_TEST_RUN)
	set(HASH_TEST_RUN 1 CACHE INTERNAL "Has run the hash map and hash set tests")
	foreach (HASH_HEADER ext/ "")
		foreach (HASH_NS stdext __gnu_cxx "" std)
			message(STATUS "Looking for hash_map and hash_set in header <${HASH_HEADER}hash_map>, <${HASH_HEADER}hash_set> and namespace '${HASH_NS}'")
			file(WRITE
				${CMAKE_BINARY_DIR}/CMakeTmp/testHashImpl.cpp
				"#include <${HASH_HEADER}hash_map>
				#include <${HASH_HEADER}hash_set>
				int main () { ${HASH_NS}::hash_map<int, int> m; ${HASH_NS}::hash_set<int> s; return 0; }"
			)
			
			try_compile(HASH_WORKS
				${CMAKE_BINARY_DIR}
				${CMAKE_BINARY_DIR}/CMakeTmp/testHashImpl.cpp
			)
			
			if (HASH_WORKS)
				message(STATUS "Looking for hash_map and hash_set in header <${HASH_HEADER}hash_map>, <${HASH_HEADER}hash_set> and namespace '${HASH_NS}' - found!")
				set(HAVE_HASH_MAP 1 CACHE INTERNAL "Have hash_map")
				set(HAVE_HASH_SET 1 CACHE INTERNAL "Have hash_set")
				set(HASH_MAP_H <${HASH_HEADER}hash_map> CACHE INTERNAL "hash_map header")
				set(HASH_SET_H <${HASH_HEADER}hash_set> CACHE INTERNAL "hash_set header")
				set(HASH_NAMESPACE ${HASH_NS} CACHE INTERNAL "Namespace for hash_map/hash_set")
			else (HASH_WORKS)
				message(STATUS "Looking for hash_map and hash_set in header <${HASH_HEADER}hash_map>, <${HASH_HEADER}hash_set> and namespace '${HASH_NS}' - not found")
			endif (HASH_WORKS)
		endforeach (HASH_NS stdext __gnu_cxx "" std)
	endforeach (HASH_HEADER ext/ "")
endif (NOT HASH_TEST_RUN)

if (NOT HAVE_HASH_MAP)
	set(HAVE_HASH_MAP "" CACHE INTERNAL "Have hash_map")
	set(HAVE_HASH_SET "" CACHE INTERNAL "Have hash_set")
	set(HASH_MAP_H "" CACHE INTERNAL "hash_map header")
	set(HASH_SET_H "" CACHE INTERNAL "hash_set header")
	set(HASH_NAMESPACE "" CACHE INTERNAL "Namespace for hash_map/hash_set")
endif (NOT HAVE_HASH_MAP)


check_include_files( dlfcn.h HAVE_DLFCN_H )
check_include_files( fcntl.h HAVE_FCNTL_H )
check_function_exists( ftruncate HAVE_FTRUNCATE ) 
check_include_files( inttypes.h HAVE_INTTYPES_H )
check_include_files( limits.h HAVE_LIMITS_H )
check_include_files( memory.h HAVE_MEMORY_H )
check_function_exists( memset HAVE_MEMSET ) 
check_function_exists( mkdir HAVE_MKDIR ) 

if (CMAKE_USE_PTHREADS_INIT)
	set(HAVE_PTHREAD 1 CACHE INTERNAL "Have POSIX threads")
else (CMAKE_USE_PTHREADS_INIT)
	set(HAVE_PTHREAD "" CACHE INTERNAL "Have POSIX threads")
endif (CMAKE_USE_PTHREADS_INIT)

check_include_files( stdint.h HAVE_STDINT_H )
check_include_files( stdlib.h HAVE_STDLIB_H )
check_function_exists( strchr HAVE_STRCHR ) 
check_function_exists( strerror HAVE_STRERROR ) 
check_include_files( strings.h HAVE_STRINGS_H )
check_include_files( string.h HAVE_STRING_H )
check_function_exists( strtol HAVE_STRTOL ) 
check_include_files( sys/stat.h HAVE_SYS_STAT_H )
check_include_files( sys/types.h HAVE_SYS_TYPES_H )
check_include_files( unistd.h HAVE_UNISTD_H )

if (ZLIB_FOUND)
	set(HAVE_ZLIB 1 CACHE INTERNAL "Have ZLIB library")
else (ZLIB_FOUND)
	set(HAVE_ZLIB "" CACHE INTERNAL "Have ZLIB library")
endif (ZLIB_FOUND)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h )


set(LIBPPROTOBUF_SOURCES 
	${PROTOBUF_DIR}/google/protobuf/stubs/common.cc
	${PROTOBUF_DIR}/google/protobuf/stubs/once.cc
	${PROTOBUF_DIR}/google/protobuf/stubs/hash.cc
	${PROTOBUF_DIR}/google/protobuf/stubs/hash.h
	${PROTOBUF_DIR}/google/protobuf/stubs/map-util.cc
	${PROTOBUF_DIR}/google/protobuf/stubs/map-util.h
	${PROTOBUF_DIR}/google/protobuf/stubs/stl_util-inl.cc
	${PROTOBUF_DIR}/google/protobuf/stubs/stl_util-inl.h
	${PROTOBUF_DIR}/google/protobuf/stubs/substitute.cc
	${PROTOBUF_DIR}/google/protobuf/stubs/substitute.h
	${PROTOBUF_DIR}/google/protobuf/stubs/strutil.cc
	${PROTOBUF_DIR}/google/protobuf/stubs/strutil.h
	${PROTOBUF_DIR}/google/protobuf/stubs/structurally_valid.cc
	${PROTOBUF_DIR}/google/protobuf/descriptor.cc
	${PROTOBUF_DIR}/google/protobuf/descriptor.pb.cc
	${PROTOBUF_DIR}/google/protobuf/descriptor_database.cc
	${PROTOBUF_DIR}/google/protobuf/dynamic_message.cc
	${PROTOBUF_DIR}/google/protobuf/extension_set.cc
	${PROTOBUF_DIR}/google/protobuf/generated_message_reflection.cc
	${PROTOBUF_DIR}/google/protobuf/message.cc
	${PROTOBUF_DIR}/google/protobuf/reflection_ops.cc
	${PROTOBUF_DIR}/google/protobuf/repeated_field.cc
	${PROTOBUF_DIR}/google/protobuf/service.cc
	${PROTOBUF_DIR}/google/protobuf/text_format.cc
	${PROTOBUF_DIR}/google/protobuf/unknown_field_set.cc
	${PROTOBUF_DIR}/google/protobuf/wire_format.cc
	${PROTOBUF_DIR}/google/protobuf/io/coded_stream.cc
	${PROTOBUF_DIR}/google/protobuf/io/gzip_stream.cc
	${PROTOBUF_DIR}/google/protobuf/io/printer.cc
	${PROTOBUF_DIR}/google/protobuf/io/tokenizer.cc
	${PROTOBUF_DIR}/google/protobuf/io/zero_copy_stream.cc
	${PROTOBUF_DIR}/google/protobuf/io/zero_copy_stream_impl.cc
	${PROTOBUF_DIR}/google/protobuf/compiler/importer.cc
	${PROTOBUF_DIR}/google/protobuf/compiler/parser.cc
)

include_directories( . ${PROTOBUF_DIR} )
add_library( protobuf STATIC ${LIBPPROTOBUF_SOURCES} )
