#ifndef __LD_DATATYPES_HPP__
#define __LD_DATATYPES_HPP__

#include <cstdint>
#include <cstddef>

typedef unsigned char	LD_BYTE;
typedef unsigned char	LD_UCHAR;
typedef signed char		LD_SCHAR;
typedef int8_t			LD_SINT8;
typedef uint8_t			LD_UINT8;
typedef int16_t			LD_SINT16;
typedef uint16_t		LD_UINT16;
typedef int32_t			LD_SINT32;
typedef uint32_t		LD_UINT32;
typedef int64_t			LD_SINT64;
typedef uint64_t		LD_UINT64;

typedef LD_UINT32	LD_BOOL;
typedef size_t		LD_MEMSIZE;
typedef float		LD_FLOAT32;
typedef double		LD_FLOAT64;

#define LD_NULL nullptr

#define LD_EXPLICIT explicit

const LD_UINT32 LD_OK		= 0x00000000;
const LD_UINT32 LD_FAIL	= 0xFFFFFFFF;

const LD_BOOL LD_TRUE		= 1;
const LD_BOOL LD_FALSE	= 0;

typedef struct _tagFILE_CHUNK
{
	LD_UINT32	ID;
	LD_UINT32	Size;
}FILE_CHUNK,*PFILE_CHUNK;

const LD_UINT32 FILE_END_CHUNK	= 0xFFFFFFFF;

#endif // __LD_DATATYPES_HPP__

