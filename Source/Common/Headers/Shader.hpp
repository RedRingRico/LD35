#ifndef __LD_SHADER_HPP__
#define __LD_SHADER_HPP__

#include <DataTypes.hpp>
#include <MD5.hpp>
#include <list>
#include <map>
#include <string>
#include <GL/glew.h>
#include <GL/gl.h>
#include <hl_md5.h>

namespace LD
{
	typedef enum _tagSHADER_TYPE
	{
		SHADER_TYPE_NONE		= 0,
		SHADER_TYPE_VERTEX		= 1,
		SHADER_TYPE_FRAGMENT	= 2,
		SHADER_TYPE_GEOMETRY	= 4,
		SHADER_TYPE_UNKNOWN		= 0x7FFFFFFF
	}SHADER_TYPE;

	typedef enum _tagSHADER_PARAMETER_TYPE
	{
		SHADER_PARAMETER_TYPE_FLOAT1	= 1,
		SHADER_PARAMETER_TYPE_FLOAT2	= 2,
		SHADER_PARAMETER_TYPE_FLOAT3	= 3,
		SHADER_PARAMETER_TYPE_FLOAT4	= 4,

		SHADER_PARAMETER_TYPE_MATRIX2X2	= 10,
		SHADER_PARAMETER_TYPE_MATRIX3X3	= 11,
		SHADER_PARAMETER_TYPE_MATRIX4X4	= 12,

		SHADER_PARAMETER_TYPE_BOOL1		= 20,
		SHADER_PARAMETER_TYPE_BOOL2		= 21,
		SHADER_PARAMETER_TYPE_BOOL3		= 22,
		SHADER_PARAMETER_TYPE_BOOL4		= 23,

		SHADER_PARAMETER_TYPE_INT1		= 30,
		SHADER_PARAMETER_TYPE_INT2		= 31,
		SHADER_PARAMETER_TYPE_INT3		= 32,
		SHADER_PARAMETER_TYPE_INT4		= 33,

		SHADER_PARAMETER_TYPE_UINT1		= 40,
		SHADER_PARAMETER_TYPE_UINT2		= 41,
		SHADER_PARAMETER_TYPE_UINT3		= 42,
		SHADER_PARAMETER_TYPE_UINT4		= 43,

		SHADER_PARAMETER_TYPE_SAMPLER_1D	= 50,
		SHADER_PARAMETER_TYPE_SAMPLER_2D	= 51,
		SHADER_PARAMETER_TYPE_SAMPLER_3D	= 52,

		SHADER_PARAMETER_TYPE_UNKNOWN	= 0x7FFFFFFF
	}SHADER_PARAMETER_TYPE;

	typedef struct _tagSHADER_PARAMETER
	{
		SHADER_PARAMETER_TYPE	Type;
		GLint					Location;
		GLsizei					ArraySize;
	}SHADER_PARAMETER,*PSHADER_PARAMETER;

	class Shader
	{
	public:
		Shader( );
		~Shader( );

		LD_UINT32 CreateShaderFromSource( const std::string &p_Source,
			const SHADER_TYPE p_ShaderType, const LD_BOOL p_FromFile );

		LD_UINT32 Apply( );

		LD_UINT32 SetShaderParameter( const std::string &p_Name,
			void *p_pValue );

		LD_UINT32 GetShaderParameters(
			std::list< std::string > &p_Names ) const;

		LD_UINT32 GetDigest( MD5_DIGEST &p_Digest ) const;

		LD_UINT32 Link( );

	private:
		LD_UINT32 ExtractUniformNames( const GLchar *p_Source );
		LD_UINT32 BindUniformNamesToLocations( );

		GLuint										m_VertexID;
		GLuint										m_FragmentID;
		GLuint										m_GeometryID;
		GLuint										m_ProgramID;
		LD_BOOL									m_Linked;
		MD5											m_MD5;
		HL_MD5_CTX									m_MD5Context;
		MD5_DIGEST									m_MD5Digest;
		std::map< std::string, SHADER_PARAMETER >	m_UniformLocationMap;
	};

	SHADER_TYPE &operator|=( SHADER_TYPE &p_Left, SHADER_TYPE p_Right );
}

#endif // __LD_SHADER_HPP__

