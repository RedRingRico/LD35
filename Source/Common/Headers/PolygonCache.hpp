#ifndef __LD_POLYGONCACHE_HPP__
#define __LD_POLYGONCACHE_HPP__

#include <DataTypes.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>

namespace LD
{
	typedef struct _POLYGONCACHE
	{
		LD_UINT32	ID;
		LD_MEMSIZE	Stride;
		LD_MEMSIZE	VertexCount;
		LD_MEMSIZE	IndexCount;
		LD_MEMSIZE	PolygonCount;
		GLuint		VertexBufferID;
		GLuint		IndexBufferID;
		GLuint		VertexArrayID;
		GLenum		PrimitiveType;
	}POLYGONCACHE,*PPOLYGONCACHE;

	class PolygonCache
	{
	public:
		PolygonCache( );
		~PolygonCache( );

		LD_UINT32 AddPolygons( const LD_MEMSIZE p_VertexCount,
			const LD_MEMSIZE p_IndexCount, const LD_BYTE *p_pVertices,
			const LD_UINT16 *p_pIndices, const GLenum p_PrimitiveType,
			const LD_UINT64 p_VertexAttributes, LD_UINT32 &p_CacheID );
		
		LD_UINT32 Render( const LD_UINT32 p_CacheID );

		LD_UINT32 DeletePolygons( const LD_UINT32 p_CacheID );

		void Purge( );

	private:
		std::vector< POLYGONCACHE >	m_Cache;

		LD_UINT32 m_CacheID;
	};
}

#endif // __LD_POLYGONCACHE_HPP__

