#ifndef FILE_H_
#define FILE_H_

namespace dlx {

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

} /*namespace dlx*/

#endif /*FILE_H_*/
