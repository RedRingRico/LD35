#include <PolygonCache.hpp>
#include <Memory.hpp>
#include <iostream>

#define BUFFER_OFFSET( Offset ) ( ( char * )LD_NULL + ( Offset ) )

namespace LD
{
	LD_MEMSIZE GetAttributeCount( const LD_UINT64 p_VertexAttributes );
	LD_MEMSIZE AttributeToSize( const LD_BYTE p_Attribute );

	PolygonCache::PolygonCache( ) :
		m_CacheID( 0U )
	{
	}

	PolygonCache::~PolygonCache( )
	{
		for( auto Itr = m_Cache.begin( ); Itr != m_Cache.end( ); ++Itr )
		{
			if( ( *Itr ).PolygonCount > 0 )
			{
				glDeleteBuffers( 1, &( *Itr ).VertexBufferID );
				glDeleteBuffers( 1, &( *Itr ).IndexBufferID );
				glDeleteVertexArrays( 1, &( *Itr ).VertexArrayID );
			}
		}
	}

	LD_UINT32 PolygonCache::AddPolygons( const LD_MEMSIZE p_VertexCount,
		const LD_MEMSIZE p_IndexCount, const LD_BYTE *p_pVertices,
		const LD_UINT16 *p_pIndices, const GLenum p_PrimitiveType,
		const LD_UINT64 p_VertexAttributes, LD_UINT32 &p_CacheID )
	{
		LD_MEMSIZE PolygonCount = 0;

		switch( p_PrimitiveType )
		{
			case GL_TRIANGLES:
			{
				PolygonCount = p_IndexCount / 3;
				break;
			}
			case GL_TRIANGLE_STRIP:
			case GL_TRIANGLE_FAN:
			{
				PolygonCount = p_IndexCount - 2;
				break;
			}
			case GL_LINES:
			{
				PolygonCount = p_IndexCount / 2;
				break;
			}
			case GL_LINE_STRIP:
			{
				PolygonCount = p_IndexCount - 1;
				break;
			}
			case GL_LINE_LOOP:
			{
				PolygonCount = p_IndexCount;
				break;
			}
		}

		if( PolygonCount == 0 )
		{
			// Can't do anything with no polygons!
			return LD_FAIL;
		}

		++m_CacheID;

		auto CacheItr = m_Cache.begin( );

		while( CacheItr != m_Cache.end( ) )
		{
			if( ( *CacheItr ).ID == 0 )
			{
				break;
			}

			++CacheItr;
		}

		POLYGONCACHE NewCache;

		NewCache.Stride = 0;
		NewCache.VertexCount = p_VertexCount;
		NewCache.IndexCount = p_IndexCount;
		NewCache.PolygonCount = PolygonCount;
		NewCache.VertexBufferID = 0;
		NewCache.IndexBufferID = 0;
		NewCache.VertexArrayID = 0;
		NewCache.PrimitiveType = p_PrimitiveType;
		NewCache.ID = m_CacheID;

		LD_MEMSIZE VertexAttributeCount =
			GetAttributeCount( p_VertexAttributes );

		for( LD_MEMSIZE Index = 0; Index < VertexAttributeCount; ++Index )
		{
			LD_BYTE Attribute =
				( p_VertexAttributes >> ( Index * 4 ) ) & 0x0F;

			NewCache.Stride += AttributeToSize( Attribute );
		}

		glGenBuffers( 1, &NewCache.VertexBufferID );
		glGenBuffers( 1, &NewCache.IndexBufferID );
		glGenVertexArrays( 1, &NewCache.VertexArrayID );

		glBindVertexArray( NewCache.VertexArrayID );
		glBindBuffer( GL_ARRAY_BUFFER, NewCache.VertexBufferID );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NewCache.IndexBufferID );

		LD_MEMSIZE TotalDimension = 0;

		for( LD_MEMSIZE VAIndex = 0; VAIndex < VertexAttributeCount;
			++VAIndex )
		{
			LD_MEMSIZE Dimension = 0;
			LD_MEMSIZE TypeSize = 0;
			GLenum Type = GL_INVALID_ENUM;
			LD_BYTE VertexAttribute = 0x0F & ( p_VertexAttributes >> (
				VAIndex * 4 ) );

			Dimension = 0x03 & ( VertexAttribute + 1 );

			switch( ( 0x0C & ( VertexAttribute ) ) >> 2 )
			{
				case 0:
				{
					Type = GL_INT;
					TypeSize = sizeof( LD_SINT32 );
					break;
				}
				case 1:
				{
					Type = GL_FLOAT;
					std::cout << "Type: Float" << std::endl;
					TypeSize = sizeof( LD_FLOAT32 );
					break;
				}
				case 2:
				{
					Type = GL_DOUBLE;
					TypeSize = sizeof( LD_FLOAT64 );
					break;
				}
				case 3:
				{
					Type = GL_FLOAT;
					TypeSize = sizeof( LD_FLOAT32 );
					Dimension *= Dimension;
					break;
				}
			}

			std::cout << "Size: " << Dimension << std::endl;

			glVertexAttribPointer( VAIndex, Dimension, Type, GL_FALSE,
				NewCache.Stride, BUFFER_OFFSET( TypeSize * TotalDimension ) );

			TotalDimension += Dimension;
			glEnableVertexAttribArray( VAIndex );
		}

		glBufferData( GL_ARRAY_BUFFER, NewCache.VertexCount * NewCache.Stride,
			p_pVertices, GL_STATIC_DRAW );

		GLenum Error = glGetError( );

		if( Error == GL_OUT_OF_MEMORY )
		{
			std::cout << "[LD::PolygonCache::AddPolygons] <ERROR> There is "
				"no more VRAM left to add vertices" << std::endl;

			glDeleteVertexArrays( 1, &( *CacheItr ).VertexArrayID );

			return LD_FAIL;
		}

		glBufferData( GL_ELEMENT_ARRAY_BUFFER,
			NewCache.IndexCount * sizeof( LD_UINT16 ), p_pIndices,
			GL_STATIC_DRAW );

		Error = glGetError( );

		if( Error == GL_OUT_OF_MEMORY )
		{
			std::cout << "[LD::PolygonCache::AddPolygons] <ERROR> There is "
				"no more VRAM left to add indices" << std::endl;
			
			glDeleteBuffers( 1, &( *CacheItr ).VertexBufferID );
			glDeleteVertexArrays( 1, &( *CacheItr ).VertexArrayID );

			return LD_FAIL;
		}

		glBindVertexArray( 0 );

		if( CacheItr == m_Cache.end( ) )
		{
			m_Cache.push_back( NewCache );
		}
		else
		{
			( *CacheItr ) = NewCache;
		}

		p_CacheID = NewCache.ID;

		return LD_OK;
	}

	LD_UINT32 PolygonCache::Render( const LD_UINT32 p_CacheID )
	{
		auto CacheItr = m_Cache.begin( );

		while( CacheItr != m_Cache.end( ) )
		{
			if( ( *CacheItr ).ID == p_CacheID )
			{
				break;
			}
			++CacheItr;
		}

		if( CacheItr == m_Cache.end( ) )
		{
			std::cout << "[LD::PolygonCache::Render] <ERROR> Could not find "
				"cache ID #" << p_CacheID << " in polygon cache" << std::endl;
			return LD_FAIL;
		}

		if( ( *CacheItr ).PolygonCount == 0 )
		{
			std::cout << "[LD::PolygonCache::Render] <ERROR> There are no "
				"polyons present in cache #" << p_CacheID;

			return LD_FAIL;
		}

		glBindVertexArray( ( *CacheItr ).VertexArrayID );
		glBindBuffer( GL_ARRAY_BUFFER, ( *CacheItr ).VertexBufferID );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ( *CacheItr ).IndexBufferID );

		glDrawElements( ( *CacheItr ).PrimitiveType, ( *CacheItr ).IndexCount,
			GL_UNSIGNED_SHORT, ( GLubyte * )LD_NULL + 0 );

		return LD_OK;
	}

	LD_MEMSIZE GetAttributeCount( const LD_UINT64 p_VertexAttributes )
	{
		LD_MEMSIZE Count = 0;

		for( LD_MEMSIZE Index = 0; Index < 16; ++Index )
		{
			// Get the nybble going from the lowest to the highest
			LD_BYTE Nybble = ( p_VertexAttributes >> ( Index * 4 ) ) & 0x0F;

			if( Nybble )
			{
				++Count;
			}
			else
			{
				break;
			}
		}

		return Count;
	}

	LD_MEMSIZE AttributeToSize( const LD_BYTE p_Attribute )
	{
		LD_BYTE Type = p_Attribute >> 2;
		LD_MEMSIZE Size = ( 0x03 & p_Attribute ) + 1;

		switch( Type )
		{
			case 0:
			{
				Size *= sizeof( LD_SINT32 );
				break;
			}
			case 1:
			{
				Size *= sizeof( LD_FLOAT32 );
				break;
			}
			case 2:
			{
				Size *= sizeof( LD_FLOAT64 );
				break;
			}
			case 3:
			{
				Size *= Size;
				Size *= sizeof( LD_FLOAT32 );
				break;
			}
		}

		return Size;
	}
}

