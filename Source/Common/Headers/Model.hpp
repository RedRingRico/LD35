#ifndef __LD_MODEL_HPP__
#define __LD_MODEL_HPP__

#include <DataTypes.hpp>
#include <MD5.hpp>
#include <string>

namespace LD
{
	class Renderer;
	class Matrix4x4;

	const LD_UINT32 MESH_CHUNK		= 0x00000001;
	const LD_UINT32 JOINT_CHUNK	= 0x00000002;

#pragma pack( 1 )
	typedef struct _tagMODEL_VERTEX
	{
		LD_FLOAT32	Position[ 3 ];
		LD_FLOAT32	Normal[ 3 ];
		LD_FLOAT32	ST[ 2 ];
		LD_UINT32	JointIndices[ 4 ];
		LD_FLOAT32	JointWeights[ 4 ];
	}MODEL_VERTEX,*PMODEL_VERTEX;

	typedef struct _tagMODEL_HEADER
	{
		char		ID[ 4 ];
		LD_UINT32	MeshCount;
		LD_UINT32	JointCount;
	}MODEL_HEADER,*PMODEL_HEADER;

	typedef struct _tagMESH_INFORMATION
	{
		char		Name[ 64 ];
		LD_UINT32	VertexCount;
		LD_UINT32	IndexCount;
		MD5_DIGEST	MaterialMD5;
	}MESH_INFORMATION,*PMESH_INFORMATION;

	typedef struct _tagMESHL_DATA
	{
		LD_UINT32	ID;
		MD5_DIGEST	MaterialMD5;
	}MESH_DATA,*PMESH_DATA;
#pragma pack( )

	class Model
	{
	public:
		Model( Renderer *p_pRenderer );
		~Model( );

		LD_UINT32 LoadFromFile( const std::string &p_FileName );

		LD_UINT32 Render( );

	private:
		LD_UINT32 ReadChunk( FILE_CHUNK &p_Chunk, FILE *p_pFile );
		LD_UINT32 ReadMeshData( FILE *p_pFile, const LD_UINT32 p_MeshIndex,
			LD_UINT32 &p_BytesRead );

		Renderer	*m_pRenderer;
		MESH_DATA	*m_pMeshData;
		LD_UINT32	m_MeshCount;
	};
}

#endif // __LD_MODEL_HPP__

