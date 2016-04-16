#include <Renderer.hpp>
#include <PolygonCache.hpp>
#include <Memory.hpp>
#include <iostream>
#include <cstring>

namespace LD
{
	Renderer::Renderer( ) :
		m_pWindow( LD_NULL ),
		m_GLContext( LD_NULL ),
		m_pPolygonCache( LD_NULL )
	{
	}

	Renderer::~Renderer( )
	{
		SDL_GL_DeleteContext( m_GLContext );
		SafeDelete< PolygonCache >( m_pPolygonCache );
		glDeleteFramebuffers( sizeof( m_GBuffer ) / sizeof( m_GBuffer[ 0 ] ),
			m_GBuffer );
		glDeleteFramebuffers( 1, &m_DepthTexture );
	}

	LD_UINT32 Renderer::Initialise( SDL_Window *p_pWindow )
	{
		GLenum Error = GL_NO_ERROR;
		if( p_pWindow == LD_NULL )
		{
			return LD_FAIL;
		}

		m_pWindow = p_pWindow;

		m_GLContext = SDL_GL_CreateContext( m_pWindow );
		
		if( m_GLContext == LD_NULL )
		{
			std::cout << "[7DLD::Renderer::Initialise] <ERROR> Failed to "
				"create an OpenGL context" << std::endl;

			SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, "7DLD Error",
				"Failed to create an OpenGL 3.2 context", LD_NULL );

			return LD_FAIL;
		}

		glewExperimental = GL_TRUE;
		GLenum GLEWError = glewInit( );

		if( GLEWError != GLEW_OK )
		{
			std::cout << "[7DLD::Renderer::Initialise] <ERROR> Failed to "
				"initialise GLEW" << std::endl;

			return LD_FAIL;
		}

		// Initialising GLEW generates an error, this is used to ensure the
		// error doesn't propagate downward
		glGetError( );

		GLint GLMajor, GLMinor;
		glGetIntegerv( GL_MAJOR_VERSION, &GLMajor );
		glGetIntegerv( GL_MINOR_VERSION, &GLMinor );

		std::cout << "[7DLD::Renderer::Initialise] <INFO> OpenGL Version: " <<
			GLMajor << "." << GLMinor << std::endl;
		
		std::cout << "[7DLD::Renderer::Initialise] <INFO> OpenGL Vendor: " <<
			glGetString( GL_VENDOR ) << std::endl;

		std::cout << "[7DLD::Renderer::Initialise] <INFO> OpenGL "
			"Renderer: " << glGetString( GL_RENDERER ) << std::endl;

		std::cout << "[7DLD::Renderer::Initialise] <INFO> GLSL Version: " <<
			glGetString( GL_SHADING_LANGUAGE_VERSION ) << std::endl;

		std::cout << "[7DLD::Renderer::Initialise] <INFO> GLEW Version: " <<
			glewGetString( GLEW_VERSION ) << std::endl;

		std::cout << "[7DLD::Renderer::Initialise] <INFO> Checking for "
			"required OpenGL extensions" << std::endl;

		GLint ExtensionCount;
		glGetIntegerv( GL_NUM_EXTENSIONS, &ExtensionCount );
		std::cout << ExtensionCount << " extensions available"
			<< std::endl;
		for( GLint Extension = 0; Extension < ExtensionCount; ++Extension )
		{
			std::cout << "\t" << Extension << ": " <<
				glGetStringi( GL_EXTENSIONS, Extension ) << std::endl;
		}

		std::cout << std::endl;

		std::cout << "\tARB_texture_storage... ";
		if( GLEW_ARB_texture_storage == GL_TRUE )
		{
			std::cout << "[OK]" << std::endl;
		}
		else
		{
			std::cout << "[!!]" << std::endl;
			return LD_FAIL;
		}

		glGenFramebuffers( 1, &m_Framebuffer );

		glBindFramebuffer( GL_FRAMEBUFFER, m_Framebuffer );

		glGenTextures( sizeof( m_GBuffer ) / sizeof( m_GBuffer[ 0 ] ),
			m_GBuffer );

		glGenTextures( 1, &m_DepthTexture );

		for( LD_MEMSIZE i = 0;
			i < sizeof( m_GBuffer ) / sizeof( m_GBuffer[ 0 ] ); ++i )
		{
			glBindTexture( GL_TEXTURE_2D, m_GBuffer[ i ] );
			/*glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, 800, 600, 0, GL_RGBA,
				GL_FLOAT, LD_NULL );*/
			glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA32F, 800, 600 );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				GL_NEAREST );
			glFramebufferTexture2D( GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_GBuffer[ i ], 0 );
		}

		glBindTexture( GL_TEXTURE_2D, m_DepthTexture );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 800, 600, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, LD_NULL );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_TEXTURE_2D, m_DepthTexture, 0 );

		GLenum DrawBuffers[ ] =
		{
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2
		};

		glDrawBuffers( sizeof( DrawBuffers ) / sizeof( DrawBuffers[ 0 ] ),
			DrawBuffers );

		Error = glCheckFramebufferStatus( GL_FRAMEBUFFER );

		if( Error != GL_FRAMEBUFFER_COMPLETE )
		{
			std::cout << "[LD::Renderer::Initialise] <ERROR> Framebuffer "
				"creation error: 0x" << std::hex <<
				static_cast< int >( Error ) << std::dec << std::endl;
			return LD_FAIL;
		}

		glBindFramebuffer( GL_FRAMEBUFFER, 0 );

		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LEQUAL );
		glViewport( 0, 0, 800, 600 );

		m_pPolygonCache = new PolygonCache( );

		return LD_OK;
	}

	LD_UINT32 Renderer::GBufferBegin( )
	{
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_Framebuffer );

		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		return LD_OK;
	}

	void Renderer::GBufferEnd( )
	{
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

		glViewport( 0, 0, 800, 600 );

		glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glBindFramebuffer( GL_READ_FRAMEBUFFER, m_Framebuffer );

		GLsizei HalfWidth = 400;
		GLsizei HalfHeight = 300;

		glReadBuffer( GL_COLOR_ATTACHMENT0 );
		glBlitFramebuffer( 0, 0, 800, 600,
			0, HalfHeight, 400, 600,
			GL_COLOR_BUFFER_BIT, GL_LINEAR );

		glReadBuffer( GL_COLOR_ATTACHMENT1 );
		glBlitFramebuffer( 0, 0, 800, 600, 0, 0, 400, 300,
			GL_COLOR_BUFFER_BIT, GL_LINEAR );

		glReadBuffer( GL_COLOR_ATTACHMENT2 );
		glBlitFramebuffer( 0, 0, 800, 600, HalfWidth, 0, 800, HalfHeight, 
			GL_COLOR_BUFFER_BIT, GL_LINEAR );

		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}

	LD_UINT32 Renderer::RegisterPolygons( const LD_MEMSIZE p_VertexCount,
		const LD_MEMSIZE p_IndexCount, const LD_BYTE *p_pVertices,
		const LD_UINT16 *p_pIndices, const GLenum p_PrimitiveType,
		const LD_UINT64 p_VertexAttributes, LD_UINT32 &p_CacheID )
	{
		return m_pPolygonCache->AddPolygons( p_VertexCount, p_IndexCount,
			p_pVertices, p_pIndices, p_PrimitiveType, p_VertexAttributes,
			p_CacheID );
	}

	LD_UINT32 Renderer::RenderPolygons( const LD_UINT32 p_CacheID )
	{
		return m_pPolygonCache->Render( p_CacheID );
	}

	void Renderer::SetClearColour( const LD_FLOAT32 p_Red,
		const LD_FLOAT32 p_Green, const LD_FLOAT32 p_Blue )
	{
		glClearColor( p_Red, p_Green, p_Blue, 1.0f );
	}

	void Renderer::Clear( const LD_BOOL p_ColourBuffer,
		const LD_BOOL p_DepthBuffer, const LD_BOOL p_StencilBuffer )
	{
		GLbitfield Flags = 0;

		if( p_ColourBuffer )
		{
			Flags |= GL_COLOR_BUFFER_BIT;
		}

		if( p_DepthBuffer )
		{
			Flags |= GL_STENCIL_BUFFER_BIT;
		}

		if( p_StencilBuffer )
		{
			Flags |= GL_STENCIL_BUFFER_BIT;
		}

		glClear( Flags );
	}

	void Renderer::SwapBuffers( )
	{
		SDL_GL_SwapWindow( m_pWindow );		
	}
}

