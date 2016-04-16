#include <Matrix4x4.hpp>
#include <Vector4.hpp>
#include <Maths.hpp>
#include <iostream>
#include <cstring>
#include <cmath>

namespace LD
{
	Matrix4x4::Matrix4x4( )
	{
		this->Identity( );
	}

	Matrix4x4::~Matrix4x4( )
	{
	}

	void Matrix4x4::Identity( )
	{
		m_M[ 0 ] = m_M[ 5 ] = m_M[ 10 ] = m_M[ 15 ] = 1.0f;
		m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 6 ] = m_M[ 7 ] =
			m_M[ 8 ] = m_M[ 9 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 14 ] = 0.0f;
	}

	Matrix4x4 &Matrix4x4::Rotate( const LD_FLOAT32 p_Angle,
		const Vector4 &p_Axis )
	{
		LD_FLOAT32 Sin = 0.0f, Cos = 0.0f, Tan = 0.0f;
		LD::SineCosine( p_Angle, Sin, Cos );

		Tan = 1.0f - Cos;

		m_M[ 0 ] = ( Tan * p_Axis.GetX( ) * p_Axis.GetX( ) ) + Cos;
		m_M[ 1 ] = ( Tan * p_Axis.GetX( ) * p_Axis.GetY( ) ) +
			( Sin + p_Axis.GetZ( ) );
		m_M[ 2 ] = ( Tan * p_Axis.GetX( ) * p_Axis.GetZ( ) ) -
			( Sin * p_Axis.GetY( ) );

		m_M[ 4 ] = ( Tan * p_Axis.GetX( ) * p_Axis.GetY( ) ) -
			( Sin * p_Axis.GetZ( ) );
		m_M[ 5 ] = ( Tan * p_Axis.GetY( ) * p_Axis.GetY( ) ) + Cos;
		m_M[ 6 ] = ( Tan * p_Axis.GetY( ) * p_Axis.GetZ( ) ) +
			( Sin * p_Axis.GetX( ) );

		m_M[ 8 ] = ( Tan * p_Axis.GetX( ) * p_Axis.GetZ( ) ) +
			( Sin * p_Axis.GetY( ) );
		m_M[ 9 ] = ( Tan * p_Axis.GetY( ) * p_Axis.GetZ( ) ) -
			( Sin * p_Axis.GetX( ) );
		m_M[ 10 ] = ( Tan * p_Axis.GetZ( ) * p_Axis.GetZ( ) ) + Cos;

		m_M[ 3 ] = m_M[ 7 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] = m_M[ 14 ] =
			0.0f;
		m_M[ 15 ] = 1.0f;

		return *this;
	}

	Matrix4x4 &Matrix4x4::RotateX( const LD_FLOAT32 p_Angle )
	{
		LD_FLOAT32 Sin = 0.0f, Cos = 0.0f;
		LD::SineCosine( p_Angle, Sin, Cos );

		m_M[ 0 ] = 1.0f;
		m_M[ 1 ] = 0.0f;
		m_M[ 2 ] = 0.0f;
		m_M[ 3 ] = 0.0f;

		m_M[ 4 ] = 0.0f;
		m_M[ 5 ] = Cos;
		m_M[ 6 ] = Sin;
		m_M[ 7 ] = 0.0f;

		m_M[ 8 ] = 0.0f;
		m_M[ 9 ] = -Sin;
		m_M[ 10 ] = Cos;
		m_M[ 11 ] = 0.0f;

		m_M[ 12 ] = 0.0f;
		m_M[ 13 ] = 0.0f;
		m_M[ 14 ] = 0.0f;
		m_M[ 15 ] = 1.0f;

		return *this;
	}

	Matrix4x4 &Matrix4x4::RotateY( const LD_FLOAT32 p_Angle )
	{
		LD_FLOAT32 Sin = 0.0f, Cos = 0.0f;
		LD::SineCosine( p_Angle, Sin, Cos );

		m_M[ 0 ] = Cos;
		m_M[ 1 ] = 0.0f;
		m_M[ 2 ] = -Sin;
		m_M[ 3 ] = 0.0f;

		m_M[ 4 ] = 0.0f;
		m_M[ 5 ] = 1.0f;
		m_M[ 6 ] = 0.0f;
		m_M[ 7 ] = 0.0f;

		m_M[ 8 ] = Sin;
		m_M[ 9 ] = 0.0f;
		m_M[ 10 ] = Cos;
		m_M[ 11 ] = 0.0f;

		m_M[ 12 ] = 0.0f;
		m_M[ 13 ] = 0.0f;
		m_M[ 14 ] = 0.0f;
		m_M[ 15 ] = 1.0f;

		return *this;
	}

	Matrix4x4 &Matrix4x4::RotateZ( const LD_FLOAT32 p_Angle )
	{
		LD_FLOAT32 Sin = 0.0f, Cos = 0.0f;
		LD::SineCosine( p_Angle, Sin, Cos );

		m_M[ 0 ] = Cos;
		m_M[ 1 ] = Sin;
		m_M[ 2 ] = 0.0f;
		m_M[ 3 ] = 0.0f;

		m_M[ 4 ] = -Sin;
		m_M[ 5 ] = Cos;
		m_M[ 6 ] = 0.0f;
		m_M[ 7 ] = 0.0f;

		m_M[ 8 ] = 0.0f;
		m_M[ 9 ] = 0.0f;
		m_M[ 10 ] = 1.0f;
		m_M[ 11 ] = 0.0f;

		m_M[ 12 ] = 0.0f;
		m_M[ 13 ] = 0.0f;
		m_M[ 14 ] = 0.0f;
		m_M[ 15 ] = 1.0f;

		return *this;
	}

	Matrix4x4 &Matrix4x4::Scale( const LD_FLOAT32 p_Scale )
	{
		m_M[ 0 ] = m_M[ 5 ] = m_M[ 10 ] = p_Scale;

		m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 6 ] = m_M[ 7 ] =
			m_M[ 8 ] = m_M[ 9 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] =
			m_M[ 14 ] = 0.0f;
		
		m_M[ 15 ] = 1.0f;

		return *this;
	}

	Matrix4x4 &Matrix4x4::Scale( const Vector4 &p_Scale )
	{
		m_M[ 0 ] = p_Scale.GetX( );
		m_M[ 5 ] = p_Scale.GetY( );
		m_M[ 10 ] = p_Scale.GetZ( );

		m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 6 ] = m_M[ 7 ] =
			m_M[ 8 ] = m_M[ 9 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] =
			m_M[ 14 ] = 0.0f;

		m_M[ 15 ] = 1.0f;

		return *this;
	}

	Matrix4x4 &Matrix4x4::ScaleX( const LD_FLOAT32 p_Scale )
	{
		this->Identity( );
		m_M[ 0 ] = p_Scale;
		
		return *this;
	}

	Matrix4x4 &Matrix4x4::ScaleY( const LD_FLOAT32 p_Scale )
	{
		this->Identity( );
		m_M[ 5 ] = p_Scale;
		
		return *this;
	}

	Matrix4x4 &Matrix4x4::ScaleZ( const LD_FLOAT32 p_Scale )
	{
		this->Identity( );
		m_M[ 10 ] = p_Scale;

		return *this;
	}

	void Matrix4x4::SetRows( const Vector4 &p_Row1, const Vector4 &p_Row2,
		const Vector4 &p_Row3, const Vector4 &p_Row4 )
	{
		m_M[ 0 ] = p_Row1.GetX( );
		m_M[ 4 ] = p_Row1.GetY( );
		m_M[ 8 ] = p_Row1.GetZ( );
		m_M[ 12 ] = p_Row1.GetW( );

		m_M[ 1 ] = p_Row2.GetX( );
		m_M[ 5 ] = p_Row2.GetY( );
		m_M[ 9 ] = p_Row2.GetZ( );
		m_M[ 13 ] = p_Row2.GetW( );

		m_M[ 2 ] = p_Row3.GetX( );
		m_M[ 6 ] = p_Row3.GetY( );
		m_M[ 10 ] = p_Row3.GetZ( );
		m_M[ 14 ] = p_Row3.GetW( );

		m_M[ 3 ] = p_Row4.GetX( );
		m_M[ 7 ] = p_Row4.GetY( );
		m_M[ 11 ] = p_Row4.GetZ( );
		m_M[ 15 ] = p_Row4.GetW( );
	}

	void Matrix4x4::SetColumns( const Vector4 &p_Column1,
		const Vector4 &p_Column2, const Vector4 &p_Column3,
		const Vector4 &p_Column4 )
	{
		m_M[ 0 ] = p_Column1.GetX( );
		m_M[ 1 ] = p_Column1.GetY( );
		m_M[ 2 ] = p_Column1.GetZ( );
		m_M[ 3 ] = p_Column1.GetW( );

		m_M[ 4 ] = p_Column2.GetX( );
		m_M[ 5 ] = p_Column2.GetY( );
		m_M[ 6 ] = p_Column2.GetZ( );
		m_M[ 7 ] = p_Column2.GetW( );

		m_M[ 8 ] = p_Column3.GetX( );
		m_M[ 9 ] = p_Column3.GetY( );
		m_M[ 10 ] = p_Column3.GetZ( );
		m_M[ 11 ] = p_Column3.GetW( );

		m_M[ 12 ] = p_Column4.GetX( );
		m_M[ 13 ] = p_Column4.GetY( );
		m_M[ 14 ] = p_Column4.GetZ( );
		m_M[ 15 ] = p_Column4.GetW( );
	}

	Matrix4x4 &Matrix4x4::CreatePerspectiveFOV( const LD_FLOAT32 p_FOV,
		const LD_FLOAT32 p_AspectRatio, const LD_FLOAT32 p_NearPlane,
		const LD_FLOAT32 p_FarPlane )
	{
		if( LD::Absolute( p_FarPlane - p_NearPlane ) < LD_EPSILON )
		{
			return *this;
		}

		this->Identity( );

		LD_FLOAT32 D = 1.0f / tanf( p_FOV / 180.0f ) * LD_PI * 0.5f;
		LD_FLOAT32 Recip = 1.0f / ( p_NearPlane - p_FarPlane );

		m_M[ 0 ] = D / p_AspectRatio;
		m_M[ 5 ] = D;
		m_M[ 10 ] = ( p_NearPlane + p_FarPlane ) * Recip;
		m_M[ 11 ] = -1.0f;
		m_M[ 14 ] = 2 * p_NearPlane * p_FarPlane * Recip;
		m_M[ 15 ] = 0.0f;

		return *this;
	}

	Matrix4x4 &Matrix4x4::CreateViewLookAt( const Vector4 &p_Position,
		const Vector4 &p_Point, const Vector4 &p_WorldUp )
	{
		Vector4 Right, Up, Direction;

		Direction = p_Point - p_Position;
		Direction.Normalise( );

		Right = Direction.CrossProduct( p_WorldUp );
		Right.Normalise( );

		Up = Right.CrossProduct( Direction );
		Up.Normalise( );

		Matrix4x4 Upper3x3;

		Vector4 Zero( 0.0f, 0.0f, 0.0f, 1.0f );
		Upper3x3.SetColumns( Right, Up, Direction, Zero );

		Vector4 Position = ( Upper3x3 * p_Position );

		this->CreateView3D( Right, Up, Direction, Position );

		return *this;
	}

	Matrix4x4 &Matrix4x4::Translate( const Vector4 &p_Translation )
	{
		m_M[ 0 ] = 1.0f;
		m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = 0.0f;
		m_M[ 5 ] = 1.0f;
		m_M[ 6 ] = m_M[ 7 ] = m_M[ 8 ] = m_M[ 9 ] = 0.0f;
		m_M[ 10 ] = 1.0f;
		m_M[ 11 ] = 0.0f;
		m_M[ 12 ] = p_Translation.GetX( );
		m_M[ 13 ] = p_Translation.GetY( );
		m_M[ 14 ] = p_Translation.GetZ( );
		m_M[ 15 ] = 1.0f;

		return *this;
	}

	LD_FLOAT32 Matrix4x4::Trace( ) const
	{
		return m_M[ 0 ] + m_M[ 5 ] + m_M[ 10 ] + m_M[ 15 ];
	}

	Matrix4x4 &Matrix4x4::CreateView3D( const Vector4 &p_Right,
		const Vector4 &p_Up, const Vector4 &p_Direction,
		const Vector4 &p_Position )
	{
		// R U D P
		// R U D P
		// R U D P
		// 0 0 0 1

		m_M[ 0 ] = p_Right.GetX( );
		m_M[ 1 ] = p_Right.GetY( );
		m_M[ 2 ] = p_Right.GetZ( );
		m_M[ 3 ] = 0.0f;

		m_M[ 4 ] = p_Up.GetX( );
		m_M[ 5 ] = p_Up.GetY( );
		m_M[ 6 ] = p_Up.GetZ( );
		m_M[ 7 ] = 0.0f;

		m_M[ 8 ] = p_Direction.GetX( );
		m_M[ 9 ] = p_Direction.GetY( );
		m_M[ 10 ] = p_Direction.GetZ( );
		m_M[ 11 ] = 0.0f;

		m_M[ 12 ] = p_Position.GetX( );
		m_M[ 13 ] = p_Position.GetY( );
		m_M[ 14 ] = p_Position.GetZ( );
		m_M[ 15 ] = 1.0f;

		return *this;
	}

	Matrix4x4 Matrix4x4::operator*( const Matrix4x4 &p_Right ) const
	{
		Matrix4x4 Multiply;

		Multiply.m_M[ 0 ] = ( m_M[ 0 ] * p_Right.m_M[ 0 ] ) +
							( m_M[ 4 ] * p_Right.m_M[ 1 ] ) +
							( m_M[ 8 ] * p_Right.m_M[ 2 ] ) +
							( m_M[ 12 ] * p_Right.m_M[ 3 ] );
		Multiply.m_M[ 1 ] =	( m_M[ 1 ] * p_Right.m_M[ 0 ] ) +
							( m_M[ 5 ] * p_Right.m_M[ 1 ] ) +
							( m_M[ 9 ] * p_Right.m_M[ 2 ] ) +
							( m_M[ 13 ] * p_Right.m_M[ 3 ] );
		Multiply.m_M[ 2 ] =	( m_M[ 2 ] * p_Right.m_M[ 0 ] ) +
							( m_M[ 6 ] * p_Right.m_M[ 1 ] ) +
							( m_M[ 10 ] * p_Right.m_M[ 2 ] ) +
							( m_M[ 14 ] * p_Right.m_M[ 3 ] );
		Multiply.m_M[ 3 ] =	( m_M[ 3 ] * p_Right.m_M[ 0 ] ) +
							( m_M[ 7 ] * p_Right.m_M[ 1 ] ) +
							( m_M[ 11 ] * p_Right.m_M[ 2 ] ) +
							( m_M[ 15 ] * p_Right.m_M[ 3 ] );

		Multiply.m_M[ 4 ] = ( m_M[ 0 ] * p_Right.m_M[ 4 ] ) +
							( m_M[ 4 ] * p_Right.m_M[ 5 ] ) +
							( m_M[ 8 ] * p_Right.m_M[ 6 ] ) +
							( m_M[ 12 ] * p_Right.m_M[ 7 ] );
		Multiply.m_M[ 5 ] =	( m_M[ 1 ] * p_Right.m_M[ 4 ] ) +
							( m_M[ 5 ] * p_Right.m_M[ 5 ] ) +
							( m_M[ 9 ] * p_Right.m_M[ 6 ] ) +
							( m_M[ 13 ] * p_Right.m_M[ 7 ] );
		Multiply.m_M[ 6 ] =	( m_M[ 2 ] * p_Right.m_M[ 4 ] ) +
							( m_M[ 6 ] * p_Right.m_M[ 5 ] ) +
							( m_M[ 10 ] * p_Right.m_M[ 6 ] ) +
							( m_M[ 14 ] * p_Right.m_M[ 7 ] );
		Multiply.m_M[ 7 ] =	( m_M[ 3 ] * p_Right.m_M[ 4 ] ) +
							( m_M[ 7 ] * p_Right.m_M[ 5 ] ) +
							( m_M[ 11 ] * p_Right.m_M[ 6 ] ) +
							( m_M[ 15 ] * p_Right.m_M[ 7 ] );

		Multiply.m_M[ 8 ] = ( m_M[ 0 ] * p_Right.m_M[ 8 ] ) +
							( m_M[ 4 ] * p_Right.m_M[ 9 ] ) +
							( m_M[ 8 ] * p_Right.m_M[ 10 ] ) +
							( m_M[ 12 ] * p_Right.m_M[ 11 ] );
		Multiply.m_M[ 9 ] =	( m_M[ 1 ] * p_Right.m_M[ 8 ] ) +
							( m_M[ 5 ] * p_Right.m_M[ 9 ] ) +
							( m_M[ 9 ] * p_Right.m_M[ 10 ] ) +
							( m_M[ 13 ] * p_Right.m_M[ 11 ] );
		Multiply.m_M[ 10 ] =	( m_M[ 2 ] * p_Right.m_M[ 8 ] ) +
							( m_M[ 6 ] * p_Right.m_M[ 9 ] ) +
							( m_M[ 10 ] * p_Right.m_M[ 10 ] ) +
							( m_M[ 14 ] * p_Right.m_M[ 11 ] );
		Multiply.m_M[ 11 ] =	( m_M[ 3 ] * p_Right.m_M[ 8 ] ) +
							( m_M[ 7 ] * p_Right.m_M[ 9 ] ) +
							( m_M[ 11 ] * p_Right.m_M[ 10 ] ) +
							( m_M[ 15 ] * p_Right.m_M[ 11 ] );

		Multiply.m_M[ 12 ] = ( m_M[ 0 ] * p_Right.m_M[ 12 ] ) +
							( m_M[ 4 ] * p_Right.m_M[ 13 ] ) +
							( m_M[ 8 ] * p_Right.m_M[ 14 ] ) +
							( m_M[ 12 ] * p_Right.m_M[ 15 ] );
		Multiply.m_M[ 13 ] =	( m_M[ 1 ] * p_Right.m_M[ 12 ] ) +
							( m_M[ 5 ] * p_Right.m_M[ 13 ] ) +
							( m_M[ 9 ] * p_Right.m_M[ 14 ] ) +
							( m_M[ 13 ] * p_Right.m_M[ 15 ] );
		Multiply.m_M[ 14 ] =	( m_M[ 2 ] * p_Right.m_M[ 12 ] ) +
							( m_M[ 6 ] * p_Right.m_M[ 13 ] ) +
							( m_M[ 10 ] * p_Right.m_M[ 14 ] ) +
							( m_M[ 14 ] * p_Right.m_M[ 15 ] );
		Multiply.m_M[ 15 ] =	( m_M[ 3 ] * p_Right.m_M[ 12 ] ) +
							( m_M[ 7 ] * p_Right.m_M[ 13 ] ) +
							( m_M[ 11 ] * p_Right.m_M[ 14 ] ) +
							( m_M[ 15 ] * p_Right.m_M[ 15 ] );

		return Multiply;
	}

	Vector4 Matrix4x4::operator*( const Vector4 &p_Vector ) const
	{
		Vector4 Multiply;

		Multiply.SetX(	( m_M[ 0 ] * p_Vector.GetX( ) ) +
						( m_M[ 4 ] * p_Vector.GetY( ) ) +
						( m_M[ 8 ] * p_Vector.GetZ( ) ) +
						( m_M[ 12 ] * p_Vector.GetW( ) ) );

		Multiply.SetY(	( m_M[ 1 ] * p_Vector.GetX( ) ) +
						( m_M[ 5 ] * p_Vector.GetY( ) ) +
						( m_M[ 9 ] * p_Vector.GetZ( ) ) +
						( m_M[ 13 ] * p_Vector.GetW( ) ) );
		
		Multiply.SetZ(	( m_M[ 2 ] * p_Vector.GetX( ) ) +
						( m_M[ 6 ] * p_Vector.GetY( ) ) +
						( m_M[ 10 ] * p_Vector.GetZ( ) ) +
						( m_M[ 14 ] * p_Vector.GetW( ) ) );

		Multiply.SetW(	( m_M[ 3 ] * p_Vector.GetX( ) ) +
						( m_M[ 7 ] * p_Vector.GetY( ) ) +
						( m_M[ 11 ] * p_Vector.GetZ( ) ) +
						( m_M[ 15 ] * p_Vector.GetW( ) ) );

		return Multiply;
	}

	LD_FLOAT32 &Matrix4x4::operator[ ]( const LD_MEMSIZE p_Index )
	{
		return m_M[ p_Index ];
	}

	LD_FLOAT32 Matrix4x4::operator[ ]( const LD_MEMSIZE p_Index ) const
	{
		return m_M[ p_Index ];
	}


	void Matrix4x4::GetAsFloatArray( LD_FLOAT32 p_Raw[ 16 ] ) const
	{
		memcpy( p_Raw, m_M, sizeof( m_M ) );
	}


	void Matrix4x4::Print( const std::string &p_Name ) const
	{
		std::cout << p_Name << std::endl;

		for( LD_MEMSIZE i = 0; i < 4; ++i )
		{
			std::cout << "| " << m_M[ i ] << " " << m_M[ i + 4 ] << " " <<
				m_M[ i + 8 ] << " " << m_M[ i + 12 ] << " |" << std::endl;
		}

		std::cout << std::endl;
	}

	Vector4 operator*( const Vector4 &p_Vector, const Matrix4x4 &p_Matrix )
	{
		Vector4 Multiply;

		Multiply.SetX(	( p_Vector.GetX( ) * p_Matrix[ 0 ] ) +
						( p_Vector.GetY( ) * p_Matrix[ 1 ] ) +
						( p_Vector.GetZ( ) * p_Matrix[ 2 ] ) +
						( p_Vector.GetW( ) * p_Matrix[ 3 ] ) );

		Multiply.SetY(	( p_Vector.GetX( ) * p_Matrix[ 4 ] ) +
						( p_Vector.GetY( ) * p_Matrix[ 5 ] ) +
						( p_Vector.GetZ( ) * p_Matrix[ 6 ] ) +
						( p_Vector.GetW( ) * p_Matrix[ 7 ] ) );

		Multiply.SetZ(	( p_Vector.GetX( ) * p_Matrix[ 8 ] ) +
						( p_Vector.GetY( ) * p_Matrix[ 9 ] ) +
						( p_Vector.GetZ( ) * p_Matrix[ 10 ] ) +
						( p_Vector.GetW( ) * p_Matrix[ 11 ] ) );

		Multiply.SetW(	( p_Vector.GetX( ) * p_Matrix[ 12 ] ) +
						( p_Vector.GetY( ) * p_Matrix[ 13 ] ) +
						( p_Vector.GetZ( ) * p_Matrix[ 14 ] ) +
						( p_Vector.GetW( ) * p_Matrix[ 15 ] ) );

		return Multiply;
	}

}

