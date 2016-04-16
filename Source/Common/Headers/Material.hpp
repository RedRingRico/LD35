#ifndef __LD_MATERIAL_HPP__
#define __LD_MATERIAL_HPP__

#include <DataTypes.hpp>
#include <MD5.hpp>
#include <list>
#include <vector>
#include <string>

namespace LD
{
	class MaterialManager;

	class Material
	{
	public:
		Material( MaterialManager *p_pMaterialManager );
		~Material( );

		LD_UINT32 GetDigest( MD5_DIGEST &p_Digest ) const;

		LD_UINT32 CreateFromFile( const std::string &p_FileName );

		LD_UINT32 SetShaderParameter( const std::string &p_Name,
			void *p_pData );

		LD_UINT32 GetShader( MD5_DIGEST &p_Digest ) const;
		LD_UINT32 GetTextures( std::vector< MD5_DIGEST > &p_Digests ) const;

	private:
		MD5_DIGEST					m_MD5Digest;
		MD5_DIGEST					m_ShaderMD5Digest;
		std::vector< MD5_DIGEST >	m_TextureMD5Digest;
		std::list< std::string >	m_ShaderParameters;
		MaterialManager				*m_pMaterialManager;
	};
}

#endif // __LD_MATERIAL_HPP__

