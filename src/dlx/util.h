#ifndef DLX_UTIL_H_
#define DLX_UTIL_H_

namespace dlx {

// Forward declaration to prevent circular dependency between headers.
class Column;

#if CPU_ARCHITECTURE == BIG_ENDIAN

#define SwapTwoBytes(data) ( \
		(((data) >> 8) & 0x00FF) | \
		(((data) << 8) & 0xFF00) )

#define SwapFourBytes(data) ( \
		(((data) >> 24) & 0x000000FF) | \
		(((data) >> 8)  & 0x0000FF00) | \
		(((data) << 8)  & 0x00FF0000) | \
		(((data) << 24) & 0xFF000000) )

#define SwapEightBytes(data) ( \
		(((data) >> 56) & 0x00000000000000FF) | \
		(((data) >> 40) & 0x000000000000FF00) | \
		(((data) >> 24) & 0x0000000000FF0000) | \
		(((data) >> 8)  & 0x00000000FF000000) | \
		(((data) << 8)  & 0x000000FF00000000) | \
		(((data) << 24) & 0x0000FF0000000000) | \
		(((data) << 40) & 0x00FF000000000000) | \
		(((data) << 56) & 0xFF00000000000000) )

#else

#define SwapTwoBytes (data) (data)
#define SwapFourBytes (data) (data)
#define SwapEightBytes (data) (data)

#endif

typedef unsigned int uint;

void panic(const char* msg);
int read_file(char* file, Column* header);

} /*namespace dlx*/


#endif /*DLX_UTIL_H_*/
