#ifndef __LD_MATERIALMANAGER_HPP__
#define __LD_MATERIALMANAGER_HPP__

#include <Shader.hpp>
#include <Material.hpp>
#include <MD5.hpp>
#include <string>
#include <map>

namespace LD
{
	typedef struct _tagMATERIAL_SHADER
	{
		std::string VertexSource;
		std::string FragmentSource;
		std::string GeometrySource;
		SHADER_TYPE	Types;
		LD_BOOL	VertexFile;
		LD_BOOL	FragmentFile;
		LD_BOOL	GeometryFile;
	}MATERIAL_SHADER,*PMATERIAL_SHADER;

	class Texture;

    class MaterialManager
    {
    public:
		MaterialManager( );
		~MaterialManager( );

		LD_UINT32 CreateMaterial( const std::string &p_FileName,
			MD5_DIGEST &p_Digest );

		LD_UINT32 ApplyMaterial( const MD5_DIGEST &p_Digest );

		LD_UINT32 SetShaderParameter( const MD5_DIGEST &p_Digest,
			const std::string &p_Name, void *p_pData );

		LD_UINT32 CreateShader( const MATERIAL_SHADER &p_ShaderInfo,
			std::list< std::string > &p_ShaderParameters,
			MD5_DIGEST &p_ShaderDigest );

		LD_UINT32 LoadTexture( const std::string &p_FileName,
			MD5_DIGEST &p_TextureDigest );

    private:
		std::map< MD5_DIGEST, Material * >	m_Materials;
		std::map< MD5_DIGEST, Shader * >	m_Shaders;
		std::map< MD5_DIGEST, Texture * >	m_Textures;
    };
}

#endif // __LD_MATERIALMANAGER_HPP__

