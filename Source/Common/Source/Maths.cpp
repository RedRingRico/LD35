#include <Maths.hpp>
#include <cmath>

namespace LD
{
	LD_BOOL IsZero( const LD_FLOAT32 p_Value )
	{
		return ( fabs( p_Value ) < LD_EPSILON );
	}

	LD_BOOL IsEqual( LD_FLOAT32 p_Left, LD_FLOAT32 p_Right )
	{
		return ( IsZero( p_Left - p_Right ) );
	}

	LD_FLOAT32 SquareRoot( const LD_FLOAT32 p_Value )
	{
		return sqrtf( p_Value );
	}

	LD_FLOAT32 InvSquareRoot( const LD_FLOAT32 p_Value )
	{
		return ( 1.0f / LD::SquareRoot( p_Value ) );
	}

	LD_FLOAT32 Absolute( const LD_FLOAT32 p_Value )
	{
		return fabs( p_Value );
	}

	void SineCosine( const LD_FLOAT32 p_Angle, LD_FLOAT32 &p_Sine,
		LD_FLOAT32 &p_Cosine )
	{
		p_Sine = sinf( p_Angle );
		p_Cosine = cosf( p_Angle );
	}
}

