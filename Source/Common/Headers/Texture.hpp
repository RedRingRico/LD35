#ifndef __LD_TEXTURE_HPP__
#define __LD_TEXTURE_HPP__

#include <DataTypes.hpp>
#include <MD5.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <hl_md5.h>
#include <string>

namespace LD
{
	// Currently, only Targa files are handled at all
#pragma pack( 1 )
	typedef struct _tagTARGA_HEADER
	{
		LD_BYTE	IDLength;
		LD_BYTE	ColourmapType;
		LD_BYTE	ImageType;
		LD_BYTE	ColourmapSpecification[ 5 ];
		LD_UINT16	X;
		LD_UINT16	Y;
		LD_UINT16	Width;
		LD_UINT16	Height;
		LD_BYTE	BitsPerPixel;
		LD_BYTE	ImageDescription;
	}TARGA_HEADER,*PTARGA_HEADER;
#pragma pack( )

	class Shader;

	class Texture
	{
	public:
		Texture( );
		~Texture( );

		LD_UINT32 LoadFromFile( const std::string &p_FileName );

		void SetActive( Shader *p_pShader );

		LD_UINT32 GetDigest( MD5_DIGEST &p_Digest ) const;

	private:
		MD5_DIGEST	m_Digest;
		MD5			m_MD5;
		HL_MD5_CTX	m_MD5Context;
		GLuint		m_TextureID;
		GLuint		m_SamplerID;
	};
}

#endif // __LD_TEXTURE_HPP__

