#include <MaterialManager.hpp>
#include <Texture.hpp>
#include <Memory.hpp>
#include <iostream>

namespace LD
{
    MaterialManager::MaterialManager( )
    {
    }

    MaterialManager::~MaterialManager( )
    {
		for( auto Itr = m_Shaders.begin( ); Itr != m_Shaders.end( ); ++Itr )
		{
			SafeDelete< Shader >( Itr->second );
		}

		for( auto Itr = m_Textures.begin( ); Itr != m_Textures.end( ); ++Itr )
		{
			SafeDelete< Texture >( Itr->second );
		}

		for( auto Itr = m_Materials.begin( ); Itr != m_Materials.end( );
			++Itr )
		{
			SafeDelete< Material >( Itr->second );
		}
    }

    LD_UINT32 MaterialManager::CreateMaterial( const std::string &p_FileName,
		MD5_DIGEST &p_Digest )
    {
		MD5_DIGEST Digest;
		MD5Zero( Digest );
		Material *pNewMaterial = new Material( this );

		if( pNewMaterial->CreateFromFile( p_FileName ) != LD_OK )
		{
			std::cout << "[LD::MaterialManager::CreateMaterial] "
				"<ERROR> Failed to create material from file" << std::endl;

			SafeDelete< Material >( pNewMaterial );

			return LD_FAIL;
		}

		if( pNewMaterial->GetDigest( Digest ) != LD_OK )
		{
			std::cout << "[LD::MaterialManager::CreateMaterial] "
				"<ERROR> Failed to obtain the material's MD5 digest" <<
				std::endl;

			SafeDelete< Material >( pNewMaterial );

			return LD_FAIL;
		}

		std::pair< std::map< MD5_DIGEST, Material * >::iterator, bool > Insert;

		Insert = m_Materials.insert( std::pair< MD5_DIGEST, Material * >(
			Digest, pNewMaterial ) );

		if( Insert.second == false )
		{
			std::cout << "[LD::MaterialManager::CreateMaterial] <INFO> "
				"Material already exists: " << MD5AsString( Digest ) <<
				std::endl;

			SafeDelete< Material >( pNewMaterial );
		}

		p_Digest = Digest;

		return LD_OK;
	}

	LD_UINT32 MaterialManager::ApplyMaterial( const MD5_DIGEST &p_Digest )
	{
		std::map< MD5_DIGEST, Material * >::const_iterator MaterialItr =
			m_Materials.begin( );

		while( MaterialItr != m_Materials.end( ) )
		{
			if( MD5Equal( MaterialItr->first, p_Digest ) )
			{
				break;
			}

			++MaterialItr;
		}

		if( MaterialItr == m_Materials.end( ) )
		{
			std::cout << "[LD::MaterialManager::ApplyMaterial] <ERROR> "
				"Failed to find material '" << MD5AsString( p_Digest ) <<
				"'" << std::endl;

			return LD_FAIL;
		}

		MD5_DIGEST ShaderDigest;

		MaterialItr->second->GetShader( ShaderDigest );

		std::map< MD5_DIGEST, Shader * >::iterator ShaderItr =
			m_Shaders.begin( );

		while( ShaderItr != m_Shaders.end( ) )
		{
			if( MD5Equal( ShaderItr->first, ShaderDigest ) )
			{
				break;
			}
			++ShaderItr;
		}

		if( ShaderItr == m_Shaders.end( ) )
		{
			std::cout << "[LD::MaterialManager::ApplyMaterial] <ERROR> "
				"Failed to find shader '" << MD5AsString( ShaderDigest ) <<
				"' for material '" << MD5AsString( p_Digest ) << "'" <<
				std::endl;

			return LD_FAIL;
		}

		std::vector< MD5_DIGEST > TextureArray;
		MaterialItr->second->GetTextures( TextureArray );

		for( LD_MEMSIZE i = 0; i < TextureArray.size( ); ++i )
		{
			auto TextureItr = m_Textures.begin( );

			while( TextureItr != m_Textures.end( ) )
			{
				if( MD5Equal( TextureItr->first, TextureArray[ i ] ) )
				{
					TextureItr->second->SetActive( ShaderItr->second );
					break;
				}
				++TextureItr;
			}

			if( TextureItr == m_Textures.end( ) )
			{
				return LD_FAIL;
			}
		}

		ShaderItr->second->Apply( );

		return LD_OK;
    }

    LD_UINT32 MaterialManager::SetShaderParameter( const MD5_DIGEST &p_Digest,
		const std::string &p_Name, void *p_pData )
    {
		std::map< MD5_DIGEST, Material * >::const_iterator MaterialItr =
			m_Materials.begin( );

		while( MaterialItr != m_Materials.end( ) )
		{
			if( MD5Equal( MaterialItr->first, p_Digest ) )
			{
				break;
			}

			++MaterialItr;
		}

		if( MaterialItr == m_Materials.end( ) )
		{
			return LD_FAIL;
		}

		MD5_DIGEST ShaderDigest;

		MaterialItr->second->GetShader( ShaderDigest );

		std::map< MD5_DIGEST, Shader * >::iterator ShaderItr =
			m_Shaders.begin( );

		while( ShaderItr != m_Shaders.end( ) )
		{
			if( MD5Equal( ShaderItr->first, ShaderDigest ) )
			{
				break;
			}
			++ShaderItr;
		}

		if( ShaderItr == m_Shaders.end( ) )
		{
			return LD_FAIL;
		}

		return ShaderItr->second->SetShaderParameter( p_Name, p_pData );
    }

    LD_UINT32 MaterialManager::CreateShader(
		const MATERIAL_SHADER &p_ShaderInfo,
		std::list< std::string > &p_ShaderParameters,
		MD5_DIGEST &p_ShaderDigest )
    {
		Shader *pMaterialShader = new Shader( );

		if( p_ShaderInfo.Types & SHADER_TYPE_VERTEX )
		{
			if( pMaterialShader->CreateShaderFromSource(
				p_ShaderInfo.VertexSource, SHADER_TYPE_VERTEX,
				p_ShaderInfo.VertexFile ) != LD_OK )
			{
				std::cout << "[LD::MaterialManager::CreateShader] <ERROR> "
					"Failed to compile the vertex shader" << std::endl;

				return LD_FAIL;
			}
		}

		if( p_ShaderInfo.Types & SHADER_TYPE_FRAGMENT )
		{
			if( pMaterialShader->CreateShaderFromSource(
				p_ShaderInfo.FragmentSource, SHADER_TYPE_FRAGMENT,
				p_ShaderInfo.FragmentFile ) != LD_OK )
			{
				std::cout << "[LD::MaterialManager::CreateShader] <ERROR> "
					"Failed to compile the vertex shader" << std::endl;

				return LD_FAIL;
			}
		}

		if( p_ShaderInfo.Types & SHADER_TYPE_GEOMETRY )
		{
			if( pMaterialShader->CreateShaderFromSource(
				p_ShaderInfo.GeometrySource, SHADER_TYPE_GEOMETRY,
				p_ShaderInfo.GeometryFile ) != LD_OK )
			{
				std::cout << "[LD::MaterialManager::CreateShader] <ERROR> "
					"Failed to compile the vertex shader" << std::endl;

				return LD_FAIL;
			}
		}

		pMaterialShader->GetShaderParameters( p_ShaderParameters );
		pMaterialShader->Link( );
		pMaterialShader->GetDigest( p_ShaderDigest );

		std::pair< std::map< MD5_DIGEST, Shader * >::iterator, bool >
			ShaderMapResult;
		
		ShaderMapResult = m_Shaders.insert( std::pair< MD5_DIGEST, Shader * >(
			p_ShaderDigest, pMaterialShader ) );

		if( ShaderMapResult.second == false )
		{
			// The shader wasn't added, so delete it
			std::cout << "[LD::MaterialManager::CreateShader] <INFO> "
				"Duplicate shader detected, not adding" << std::endl;
			SafeDelete< Shader >( pMaterialShader );
		}

		return LD_OK;
	}

	LD_UINT32 MaterialManager::LoadTexture( const std::string &p_FileName,
		MD5_DIGEST &p_TextureDigest )
	{
		Texture *pMaterialTexture = new Texture( );

		if( pMaterialTexture->LoadFromFile( p_FileName ) != LD_OK )
		{
			return LD_FAIL;
		}

		pMaterialTexture->GetDigest( p_TextureDigest );

		std::pair< std::map< MD5_DIGEST, Texture * >::iterator, bool >
			TextureMapResult;

		TextureMapResult = m_Textures.insert(
			std::pair< MD5_DIGEST, Texture * >( p_TextureDigest,
				pMaterialTexture ) );

		if( TextureMapResult.second == false )
		{
			std::cout << "[LD::MaterialManager::LoadTexture] <INFO> "
				"Duplicate texture detected, not adding" << std::endl;
			SafeDelete< Texture >( pMaterialTexture );
		}

		return LD_OK;
	}
}

