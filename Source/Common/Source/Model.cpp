#include <Model.hpp>
#include <Renderer.hpp>
#include <Memory.hpp>
#include <iostream>
#include <cstdio>

namespace LD
{
	Model::Model( Renderer *p_pRenderer ) :
		m_pRenderer( p_pRenderer ),
		m_pMeshData( LD_NULL ),
		m_MeshCount( 0U )
	{
	}

	Model::~Model( )
	{
		SafeDeleteArray< MESH_DATA >( m_pMeshData );
	}

	LD_UINT32 Model::LoadFromFile( const std::string &p_FileName )
	{
		FILE *pModelFile = fopen( p_FileName.c_str( ), "rb" );

		if( pModelFile == LD_NULL )
		{
			std::cout << "[LD::Model::LoadFromFile] <ERROR> Failed to open "
				"file " << p_FileName << " for reading" << std::endl;

			return LD_FAIL;
		}

		fseek( pModelFile, 0, SEEK_END );
		LD_UINT32 BytesToGo = ftell( pModelFile );
		rewind( pModelFile );

		// TODO
		// Get the header
		// Then get the meta data
		// Then the meshes
		// Finally, the joints
		MODEL_HEADER ModelHeader;
		fread( &ModelHeader, sizeof( ModelHeader ), 1, pModelFile );

		BytesToGo -= sizeof( ModelHeader );

		m_MeshCount = ModelHeader.MeshCount;
		m_pMeshData = new MESH_DATA[ m_MeshCount ];

		FILE_CHUNK Chunk;
		fread( &Chunk, sizeof( FILE_CHUNK ), 1, pModelFile );

		LD_UINT32 ChunkSkip = sizeof( FILE_CHUNK );
		LD_UINT32 BytesRead = 0U;
		LD_UINT32 MeshIndex = 0;

		// Get chunks
		while( BytesToGo )
		{
			BytesToGo -= ChunkSkip;
			if( ReadChunk( Chunk, pModelFile ) != LD_OK )
			{
				return LD_FAIL;
			}

			switch( Chunk.ID )
			{
				case MESH_CHUNK:
				{
					if( MeshIndex > ( m_MeshCount - 1 ) )
					{
						std::cout << "[LD::Model::LoadFromFile] <ERORR> "
							"An unexpected mesh was encountered (only " <<
							m_MeshCount << " meshes in the model)" <<
							std::endl;

						return LD_FAIL;
					}

					if( ReadMeshData( pModelFile, MeshIndex, BytesRead ) !=
						LD_OK )
					{
						std::cout << "[LD::Model::LoadFromFile] <ERROR> "
							"Failed to read mesh data" << std::endl;

						return LD_FAIL;
					}
					break;
				}
				default:
				{
					std::cout << "[LD::Model::LoadFromFile] <ERROR> Unknown "
						"chunk: " << Chunk.ID << std::endl;

					return LD_FAIL;
				}
			}
			BytesToGo -= BytesRead;
		}

		if( pModelFile )
		{
			fclose( pModelFile );
			pModelFile = LD_NULL;
		}

		return LD_FAIL;
	}

	LD_UINT32 Model::Render( )
	{
		std::cout << "[LD::Model::Render] <ERROR> NOT IMPLEMENTED" <<
			std::endl;

		return LD_FAIL;
	}

	LD_UINT32 Model::ReadChunk( FILE_CHUNK &p_Chunk, FILE *p_pFile )
	{
		fread( &p_Chunk, sizeof( p_Chunk ), 1, p_pFile );

		return LD_OK;
	}

	LD_UINT32 Model::ReadMeshData( FILE *p_pFile,
		const LD_UINT32 p_MeshIndex, LD_UINT32 &p_BytesRead )
	{
		LD_UINT32 BytesRead = 0;

		MESH_INFORMATION MeshInfo;
		fread( &MeshInfo, sizeof( MeshInfo ), 1, p_pFile );
		BytesRead += sizeof( MeshInfo );

		LD_BYTE *pVertices = new LD_BYTE[ MeshInfo.VertexCount ];
		fread( pVertices, sizeof( MODEL_VERTEX ), MeshInfo.VertexCount,
			p_pFile );
		BytesRead += sizeof( MODEL_VERTEX ) * MeshInfo.VertexCount;

		LD_UINT16 *pIndices = new LD_UINT16[ MeshInfo.IndexCount ];
		fread( pIndices, sizeof( LD_UINT16 ), MeshInfo.IndexCount, p_pFile );
		BytesRead += sizeof( LD_UINT16 ) * MeshInfo.IndexCount;

		m_pRenderer->RegisterPolygons( MeshInfo.VertexCount,
			MeshInfo.IndexCount, pVertices, pIndices, GL_TRIANGLES,
			0xDC566, m_pMeshData[ p_MeshIndex ].ID );

		m_pMeshData[ p_MeshIndex ].MaterialMD5 = MeshInfo.MaterialMD5;

		FILE_CHUNK Chunk;
		if( ReadChunk( Chunk, p_pFile ) != LD_OK )
		{
			std::cout << "[LD::Model::ReadMeshData] <ERROR> Failed to read "
				"end chunk" << std::endl;

			return LD_FAIL;
		}

		if( Chunk.ID != FILE_END_CHUNK )
		{
			std::cout << "[LD::Model::ReadMeshData] <ERROR> End chunk ID is "
				"not correct" << std::endl;

			return LD_FAIL;
		}

		if( Chunk.Size != 0 )
		{
			std::cout << "[LD::Model::ReadMeshData] <ERROR> End chunk size "
				"is not correct" << std::endl;

			return LD_FAIL;
		}

		BytesRead += sizeof( Chunk );

		p_BytesRead = BytesRead;

		return LD_OK;
	}
}

