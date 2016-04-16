#ifndef __LD_RENDERER_HPP__
#define __LD_RENDERER_HPP__

#include <DataTypes.hpp>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>

namespace LD
{
	class PolygonCache;

	enum GBUFFER_TYPE
	{
		GBUFFER_TYPE_POSITION = 0,
		GBUFFER_TYPE_ALBEDO,
		GBUFFER_TYPE_TEXCOORD,
		GBUFFER_TOTAL
	};

	class Renderer
	{
	public:
		Renderer( );
		~Renderer( );

		LD_UINT32 Initialise( SDL_Window *p_pWindow );

		LD_UINT32 RegisterPolygons( const LD_MEMSIZE p_VertexCount,
			const LD_MEMSIZE p_IndexCount, const LD_BYTE *p_pVertices,
			const LD_UINT16 *p_pIndices, const GLenum p_PrimitiveType,
			const LD_UINT64 p_VertexAttributes, LD_UINT32 &p_CacheID );

		LD_UINT32 GBufferBegin( );
		void GBufferEnd( );

		LD_UINT32 RenderPolygons( const LD_UINT32 p_CacheID );

		void SetClearColour( const LD_FLOAT32 p_Red,
			const LD_FLOAT32 p_Green, const LD_FLOAT32 p_Blue );

		void Clear( const LD_BOOL p_ColourBuffer,
			const LD_BOOL p_DepthBuffer, const LD_BOOL p_StencilBuffer );
		void SwapBuffers( );

	private:
		SDL_Window		*m_pWindow;
		SDL_GLContext	m_GLContext;
		PolygonCache	*m_pPolygonCache;

		GLuint			m_GBuffer[ GBUFFER_TOTAL ];
		GLuint			m_DepthTexture;
		GLuint			m_Framebuffer;
	};
}

#endif // __LD_RENDERER_HPP__

