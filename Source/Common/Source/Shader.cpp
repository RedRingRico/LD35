#include <Shader.hpp>
#include <Memory.hpp>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

namespace LD
{
	Shader::Shader( ) :
		m_VertexID( 0 ),
		m_FragmentID( 0 ),
		m_GeometryID( 0 ),
		m_ProgramID( 0 ),
		m_Linked( LD_FALSE )
	{
		MD5Zero( m_MD5Digest );

		m_MD5.MD5Init( &m_MD5Context );
	}

	Shader::~Shader( )
	{
		std::cout << "Deleting shader" << std::endl;
		if( m_ProgramID )
		{
			glDeleteProgram( m_ProgramID );
		}

		if( m_VertexID )
		{
			glDeleteShader( m_VertexID );
		}

		if( m_FragmentID )
		{
			glDeleteShader( m_FragmentID );
		}

		if( m_GeometryID )
		{
			glDeleteShader( m_GeometryID );
		}
	}

	LD_UINT32 Shader::CreateShaderFromSource( const std::string &p_Source,
		const SHADER_TYPE p_ShaderType, const LD_BOOL p_FromFile )
	{
		if( p_Source.size( ) == 0 )
		{
			std::cout << "[LD::Shader::CreateShaderFromSource] <ERROR> "
				"Source passed in was of zero length" << std::endl;

			return LD_FAIL;
		}

		GLuint ShaderID = 0;
		std::string ShaderTypeName;

		switch( p_ShaderType )
		{
			case SHADER_TYPE_VERTEX:
			{
				if( m_VertexID == 0 )
				{
					m_VertexID = glCreateShader( GL_VERTEX_SHADER );
					if( glIsShader( m_VertexID ) != GL_TRUE )
					{
						return LD_FAIL;
					}

					ShaderID = m_VertexID;
					ShaderTypeName = "Vertex";
				}
				break;
			}
			case SHADER_TYPE_FRAGMENT:
			{
				if( m_FragmentID == 0 )
				{
					m_FragmentID = glCreateShader( GL_FRAGMENT_SHADER );
					if( glIsShader( m_FragmentID ) != GL_TRUE )
					{
						return LD_FAIL;
					}

					ShaderID = m_FragmentID;
					ShaderTypeName = "Fragment";
				}
				break;
			}
			case SHADER_TYPE_GEOMETRY:
			{
				if( m_GeometryID == 0 )
				{
					m_GeometryID = glCreateShader( GL_GEOMETRY_SHADER );
					if( glIsShader( m_GeometryID ) != GL_TRUE )
					{
						return LD_FAIL;
					}

					ShaderID = m_GeometryID;
					ShaderTypeName = "Geometry";
				}
				break;
			}
			default:
			{
				std::cout << "[LD::Shader::CreateShaderFromSource] <ERROR> "
					"Unknown shader type" << std::endl;

				return LD_FAIL;
			}
		}

		if( glIsShader( ShaderID ) != GL_TRUE )
		{
			return LD_FAIL;
		}

		GLchar *pSource = LD_NULL;
		GLint SourceLength = 0;

		if( p_FromFile == LD_FALSE )
		{
			SourceLength = p_Source.size( );
			pSource = new GLchar[ SourceLength ];
			memcpy( pSource, p_Source.c_str( ), SourceLength );
		}
		else
		{
			FILE *pFile = fopen( p_Source.c_str( ), "rb" );

			fseek( pFile, 0, SEEK_END );
			SourceLength = ftell( pFile );
			rewind( pFile );

			pSource = new GLchar[ SourceLength ];

			fread( pSource, sizeof( GLchar ), SourceLength, pFile );

			if( pFile )
			{
				fclose( pFile );
				pFile = LD_NULL;
			}
		}

		m_MD5.MD5Update( &m_MD5Context,
			reinterpret_cast< unsigned char * >( pSource ),
			strlen( pSource ) );

		glShaderSource( ShaderID, 1,
			const_cast< const GLchar ** >( &pSource ), &SourceLength );

		glCompileShader( ShaderID );

		GLint CompileStatus = 0;

		glGetShaderiv( ShaderID, GL_COMPILE_STATUS, &CompileStatus );

		if( !CompileStatus )
		{
			GLint LogLength = 0;
			GLchar *pLog = LD_NULL;

			glGetShaderiv( ShaderID, GL_INFO_LOG_LENGTH, &LogLength );

			std::cout << "[LD::Shader::CreateShaderFromSource] <ERROR> " <<
				ShaderTypeName << " Shader Log:" << std::endl;

			pLog = new GLchar[ LogLength ];
			glGetShaderInfoLog( ShaderID, LogLength, &LogLength, pLog );
			std::cout << pLog << std::endl << std::endl;

			SafeDeleteArray< GLchar >( pLog );

			SafeDeleteArray< GLchar >( pSource );

			return LD_FAIL;
		}

		if( this->ExtractUniformNames( pSource ) != LD_OK )
		{
			std::cout << "[LD::Shader::CreateShaderFromSource] <ERROR> "
				"Unable to extract uniform names correctly" << std::endl;

			SafeDeleteArray< GLchar >( pSource );

			return LD_FAIL;
		}

		SafeDeleteArray< GLchar >( pSource );

		return LD_OK;
	}

	LD_UINT32 Shader::Apply( )
	{
		if( m_Linked == LD_FALSE )
		{
			if( this->Link( ) != LD_OK )
			{
				return LD_FAIL;
			}
		}

		glUseProgram( m_ProgramID );

		return LD_OK;
	}

	LD_UINT32 Shader::SetShaderParameter( const std::string &p_Name,
		void *p_pData )
	{
		SHADER_PARAMETER Parameter;
		std::map< std::string, SHADER_PARAMETER >::iterator Name =
			m_UniformLocationMap.find( p_Name );

		if( Name == m_UniformLocationMap.end( ) )
		{
			std::cout << "[LD::Shader::SetShaderParameter] <ERROR> Could not "
				"find parameter name '" << p_Name << "'" << std::endl;

			return LD_FAIL;
		}

		Parameter = Name->second;

		switch( Parameter.Type )
		{
			case SHADER_PARAMETER_TYPE_FLOAT1:
			{
				glUniform1fv( Parameter.Location, Parameter.ArraySize,
					static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case SHADER_PARAMETER_TYPE_FLOAT2:
			{
				glUniform2fv( Parameter.Location, Parameter.ArraySize,
					static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case SHADER_PARAMETER_TYPE_FLOAT3:
			{
				glUniform3fv( Parameter.Location, Parameter.ArraySize,
					static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case SHADER_PARAMETER_TYPE_FLOAT4:
			{
				glUniform4fv( Parameter.Location, Parameter.ArraySize,
					static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case SHADER_PARAMETER_TYPE_MATRIX2X2:
			{
				glUniformMatrix2fv( Parameter.Location, Parameter.ArraySize,
					GL_FALSE, static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case SHADER_PARAMETER_TYPE_MATRIX3X3:
			{
				glUniformMatrix3fv( Parameter.Location, Parameter.ArraySize,
					GL_FALSE, static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case SHADER_PARAMETER_TYPE_MATRIX4X4:
			{
				glUniformMatrix4fv( Parameter.Location, Parameter.ArraySize,
					GL_FALSE, static_cast< const GLfloat * >( p_pData ) );
				break;
			}
			case SHADER_PARAMETER_TYPE_INT1:
			case SHADER_PARAMETER_TYPE_BOOL1:
			{
				glUniform1iv( Parameter.Location, Parameter.ArraySize,
					static_cast< GLint * >( p_pData ) );
				break;
			}
			case SHADER_PARAMETER_TYPE_INT2:
			case SHADER_PARAMETER_TYPE_BOOL2:
			{
				glUniform2iv( Parameter.Location, Parameter.ArraySize,
					static_cast< GLint * >( p_pData ) );
				break;
			}
			case SHADER_PARAMETER_TYPE_INT3:
			case SHADER_PARAMETER_TYPE_BOOL3:
			{
				glUniform3iv( Parameter.Location, Parameter.ArraySize,
					static_cast< GLint * >( p_pData ) );
				break;
			}
			case SHADER_PARAMETER_TYPE_INT4:
			case SHADER_PARAMETER_TYPE_BOOL4:
			{
				glUniform4iv( Parameter.Location, Parameter.ArraySize,
					static_cast< GLint * >( p_pData ) );
				break;
			}
			case SHADER_PARAMETER_TYPE_UINT1:
			{
				glUniform1uiv( Parameter.Location, Parameter.ArraySize,
					static_cast< GLuint * >( p_pData ) );
				break;
			}
			case SHADER_PARAMETER_TYPE_UINT2:
			{
				glUniform2uiv( Parameter.Location, Parameter.ArraySize,
					static_cast< GLuint * >( p_pData ) );
				break;
			}
			case SHADER_PARAMETER_TYPE_UINT3:
			{
				glUniform3uiv( Parameter.Location, Parameter.ArraySize,
					static_cast< GLuint * >( p_pData ) );
				break;
			}
			case SHADER_PARAMETER_TYPE_UINT4:
			{
				glUniform4uiv( Parameter.Location, Parameter.ArraySize,
					static_cast< GLuint * >( p_pData ) );
				break;
			}
			case SHADER_PARAMETER_TYPE_SAMPLER_1D:
			case SHADER_PARAMETER_TYPE_SAMPLER_2D:
			case SHADER_PARAMETER_TYPE_SAMPLER_3D:
			{
				glUniform1iv( Parameter.Location, Parameter.ArraySize,
					static_cast< GLint * >( p_pData ) );
				break;
			}
			case SHADER_PARAMETER_TYPE_UNKNOWN:
			default:
			{
				return LD_FAIL;
			}
		}

		return LD_OK;
	}

	LD_UINT32 Shader::GetShaderParameters(
		std::list< std::string > &p_Names ) const
	{
		p_Names.clear( );

		auto MapItr = m_UniformLocationMap.begin( );

		while( MapItr != m_UniformLocationMap.end( ) )
		{
			p_Names.push_back( MapItr->first );
			++MapItr;
		}

		return LD_OK;
	}

	LD_UINT32 Shader::GetDigest( MD5_DIGEST &p_Digest ) const
	{
		if( m_Linked )
		{
			memcpy( &p_Digest, &m_MD5Digest, sizeof( m_MD5Digest ) );

			return LD_OK;
		}

		return LD_FAIL;
	}

	LD_UINT32 Shader::ExtractUniformNames( const GLchar *p_pSource )
	{
		// Look for the keyword "uniform" which should be followed by
		// whitespace until a built-in keyword for a vector, matrix or scalar
		// type is found.  The next part of the line should be whitespace,
		// followed by the uniform's name, possibly more whitespace, then a
		// semi-colon indicating the end of the line
		std::string SourceCopy( p_pSource );

		LD_MEMSIZE UniformPosition = SourceCopy.find( "uniform" );

		while( UniformPosition != std::string::npos )
		{
			// Get the first set of non-whitespace characters before the semi-
			// colon
			LD_MEMSIZE DelimiterPosition = SourceCopy.find_first_of( ",;",
				UniformPosition );

			if( DelimiterPosition == std::string::npos )
			{
				std::cout << "[LD::Shader::ExtractUniformNames] <ERROR> "
					"Semi-colon was not found in string" << std::endl;
				// This can't be correct
				return LD_FAIL;
			}

			// Step 1: Get the type
			LD_MEMSIZE TypeStart = SourceCopy.find_first_of( " \t",
				UniformPosition );
			// Get rid of any leading whitespace
			TypeStart = SourceCopy.find_first_not_of( " \t", TypeStart );
			LD_MEMSIZE TypeEnd = SourceCopy.find_first_of( " \t", TypeStart );

			std::string Type = SourceCopy.substr( TypeStart,
				TypeEnd - TypeStart );

			SHADER_PARAMETER ShaderParameter =
				{ SHADER_PARAMETER_TYPE_UNKNOWN, 0, 1 };

			if( Type.compare( "float" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_FLOAT1;
			}
			else if( Type.compare( "vec2" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_FLOAT2;
			}
			else if( Type.compare( "vec3" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_FLOAT3;
			}
			else if( Type.compare( "vec4" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_FLOAT4;
			}
			else if( Type.compare( "bool" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_BOOL1;
			}
			else if( Type.compare( "bvec2" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_BOOL2;
			}
			else if( Type.compare( "bvec3" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_BOOL3;
			}
			else if( Type.compare( "bvec4" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_BOOL4;
			}
			else if( Type.compare( "int" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_INT1;
			}
			else if( Type.compare( "ivec2" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_INT2;
			}
			else if( Type.compare( "ivec3" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_INT3;
			}
			else if( Type.compare( "ivec4" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_INT4;
			}
			else if( Type.compare( "uint" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_UINT1;
			}
			else if( Type.compare( "uvec2" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_UINT2;
			}
			else if( Type.compare( "uvec3" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_UINT3;
			}
			else if( Type.compare( "uvec4" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_UINT4;
			}
			else if(	( Type.compare( "mat2" ) == 0 ) ||
						( Type.compare( "mat2x2" ) == 0 ) )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_MATRIX2X2;
			}
			else if(	( Type.compare( "mat3" ) == 0 ) ||
						( Type.compare( "mat3x3" ) == 0 ) )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_MATRIX3X3;
			}
			else if(	( Type.compare( "mat4" ) == 0 ) ||
						( Type.compare( "mat4x4" ) == 0 ) )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_MATRIX4X4;
			}
			else if( Type.compare( "sampler1D" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_SAMPLER_1D;
			}
			else if( Type.compare( "sampler2D" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_SAMPLER_2D;
			}
			else if( Type.compare( "sampler3D" ) == 0 )
			{
				ShaderParameter.Type = SHADER_PARAMETER_TYPE_SAMPLER_3D;
			}
			else
			{
				std::cout << "[LD::Shader::ExtractUniformNames] <ERROR> "
					"Unknown parameter type: '" << Type << "'" << std::endl;

				return LD_FAIL;
			}

			// Step 2: Find the name
			LD_MEMSIZE UniformNameStart = SourceCopy.find_first_not_of( " \t",
				TypeEnd );
			UniformNameStart = SourceCopy.find_first_not_of( " \t",
				UniformNameStart );
			LD_MEMSIZE UniformNameEnd = SourceCopy.find_first_of( ", [;\t",
				UniformNameStart );
			LD_MEMSIZE CommaLocation = SourceCopy.find_first_of( ",",
				UniformNameStart );
			LD_MEMSIZE EndOfLine = SourceCopy.find_first_of( ';',
				UniformNameStart );

			LD_BOOL DoneProcessing = LD_FALSE;

			// Step 3: If we're not finished, check for more uniforms as well
			// as uniforms with arrays
			do
			{
				// Step 3a: Get the uniform's name
				std::string UniformName = SourceCopy.substr( UniformNameStart,
					UniformNameEnd - UniformNameStart );

				// Step 3b: Determine if the parameter is an array
				LD_MEMSIZE ArrayStart = SourceCopy.find_first_of( '[',
					UniformNameEnd );

				LD_MEMSIZE LastItem = SourceCopy.find_first_not_of( " \t",
					UniformNameEnd );
				LD_MEMSIZE ArrayEnd = 0;
				LD_BOOL FoundArray = LD_FALSE;

				if( ( ArrayStart < EndOfLine ) &&
					( ArrayStart != std::string::npos ) )
				{
					ArrayStart = SourceCopy.find_first_not_of( " \t",
						ArrayStart + 1 );
					ArrayEnd = SourceCopy.find_first_of( ']',
						ArrayStart );

					if( ( ArrayStart < DelimiterPosition ) ||
						( ArrayStart < CommaLocation ) )
					{
						if( ( ArrayEnd != std::string::npos ) &&
							( ( ArrayEnd < DelimiterPosition ) ||
								( ArrayEnd < CommaLocation ) ) )
						{
							LD_MEMSIZE ArrayEndTmp = SourceCopy.find_first_of(
								" \t", ArrayStart, ArrayEnd - ArrayStart );

							LastItem = SourceCopy.find_first_not_of( " \t",
								ArrayEnd + 1 );

							if( ArrayEndTmp != std::string::npos )
							{
								ArrayEnd = ArrayEndTmp;
							}

							FoundArray = LD_TRUE;

						}
					}
				}

				if( FoundArray )
				{
					std::string ArrayString = SourceCopy.substr( ArrayStart,
						ArrayEnd - ArrayStart );
					ShaderParameter.ArraySize = atoi( ArrayString.c_str( ) );
				}
				else
				{
					ShaderParameter.ArraySize = 1;
				}

				// Step 3c: Use the comma to determine if there are more to
				// check
				if( CommaLocation < EndOfLine )
				{
					LastItem = SourceCopy.find_first_not_of( " \t",
						CommaLocation );
				}

				std::pair< std::map< std::string,
					SHADER_PARAMETER >::iterator, bool > InsertResult;

				UniformName = SourceCopy.substr( UniformNameStart,
					UniformNameEnd - UniformNameStart );

				InsertResult = m_UniformLocationMap.insert(
					std::pair< std::string, SHADER_PARAMETER >( UniformName,
						ShaderParameter ) );

				if( InsertResult.second == false )
				{
					std::cout << "[LD::Shader::ExtractUniformNames] <WARN> "
						"Duplicate value for " << UniformName << " found" <<
						std::endl;
				}
				
				if( LastItem == EndOfLine )
				{
					DoneProcessing = LD_TRUE;

					continue;
				}

				// Get the next uniform and comma location
				UniformNameStart = SourceCopy.find_first_not_of( " \t",
					LastItem + 1 );
				UniformNameStart = SourceCopy.find_first_not_of( " \t",
					UniformNameStart );
				UniformNameEnd = SourceCopy.find_first_of( ", [;\t",
					UniformNameStart );

				CommaLocation = SourceCopy.find_first_of( ",",
					UniformNameStart );
			}while( DoneProcessing == LD_FALSE );

			UniformPosition = SourceCopy.find( "uniform", DelimiterPosition );
		}

		std::cout << "\n";
		
		return LD_OK;
	}

	LD_UINT32 Shader::Link( )
	{
		if( m_Linked )
		{
			// The program should be destroyed before being created
		}

		m_ProgramID = glCreateProgram( );

		if( glIsProgram( m_ProgramID ) != GL_TRUE )
		{
			std::cout << "[LD::Shader::Link] <ERROR> Unable to create shader "
				"program" << std::endl;

			return LD_FAIL;
		}

		// Attach shaders
		if( glIsShader( m_VertexID ) )
		{
			glAttachShader( m_ProgramID, m_VertexID );
		}

		if( glIsShader( m_FragmentID ) )
		{
			glAttachShader( m_ProgramID, m_FragmentID );
		}

		if( glIsShader( m_GeometryID ) )
		{
			glAttachShader( m_ProgramID, m_GeometryID );
		}

		glLinkProgram( m_ProgramID );

		GLint Error;
		glGetProgramiv( m_ProgramID, GL_LINK_STATUS, &Error );

		if( Error == GL_FALSE )
		{
			GLsizei Length = 0;
			GLchar *pLog;

			glGetProgramiv( m_ProgramID, GL_INFO_LOG_LENGTH, &Length );
			pLog = new GLchar[ Length ];

			glGetProgramInfoLog( m_ProgramID, Length, &Length, pLog );

			std::cout << "[LD::Shader::Link] <ERROR> Failed to link program:"
				<< std::endl;
			std::cout << pLog << std::endl;

			SafeDeleteArray< GLchar >( pLog );

			return LD_FAIL;
		}

		if( this->BindUniformNamesToLocations( ) != LD_OK )
		{
			return LD_FAIL;
		}

		m_Linked = LD_TRUE;

		m_MD5.MD5Final( m_MD5Digest.Digest, &m_MD5Context );

		return LD_OK;
	}

	LD_UINT32 Shader::BindUniformNamesToLocations( )
	{
		std::map< std::string, SHADER_PARAMETER >::iterator UniformItr =
			m_UniformLocationMap.begin( );

		while( UniformItr != m_UniformLocationMap.end( ) )
		{
			GLuint Location = glGetUniformLocation( m_ProgramID,
				UniformItr->first.c_str( ) );

			switch( Location )
			{
				case GL_INVALID_VALUE:
				{
					std::cout << "[LD::Shader::BindUniformNamesToLocations] "
						"<ERROR> program was not generated by OpenGL" <<
						std::endl;

					return LD_FAIL;
				}
				case GL_INVALID_OPERATION:
				{
					std::cout << "[LD::Shader::BindUniformNamesToLocations] "
						"<ERROR> Program object not valid" << std::endl;

					return LD_FAIL;
				}
				default:
				{
					UniformItr->second.Location = Location;
					break;
				}
			}

			std::cout << "[INFO> Bound " << UniformItr->first.c_str( ) << std::endl;

			++UniformItr;
		}

		return LD_OK;
	}

	SHADER_TYPE &operator|=( SHADER_TYPE &p_Left, SHADER_TYPE p_Right )
	{
		p_Left = static_cast< SHADER_TYPE >( static_cast< int >( p_Left ) |
			static_cast< int >( p_Right ) );

		return p_Left;
	}
}

